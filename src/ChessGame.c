#include <stdio.h>
#include <stdlib.h>
#include "ChessGame.h"


void initChessBoard(ChessGame *game) {
    game->board[2][0].type = game->board[2][1].type = game->board[2][2].type = game->board[2][3].type =
    game->board[2][4].type = game->board[2][5].type = game->board[2][6].type = game->board[2][7].type =
    game->board[3][0].type = game->board[3][1].type = game->board[3][2].type = game->board[3][3].type =
    game->board[3][4].type = game->board[3][5].type = game->board[3][6].type = game->board[3][7].type =
    game->board[4][0].type = game->board[4][1].type = game->board[4][2].type = game->board[4][3].type =
    game->board[4][4].type = game->board[4][5].type = game->board[4][6].type = game->board[4][7].type =
    game->board[5][0].type = game->board[5][1].type = game->board[5][2].type = game->board[5][3].type =
    game->board[5][4].type = game->board[5][5].type = game->board[5][6].type = game->board[5][7].type = CHESS_PIECE_TYPE_NONE;
    game->board[1][0].type = game->board[1][1].type = game->board[1][2].type = game->board[1][3].type =
    game->board[1][4].type = game->board[1][5].type = game->board[1][6].type = game->board[1][7].type = CHESS_PIECE_TYPE_PAWN;
    game->board[6][0].type = game->board[6][1].type = game->board[6][2].type = game->board[6][3].type =
    game->board[6][4].type = game->board[6][5].type = game->board[6][6].type = game->board[6][7].type = CHESS_PIECE_TYPE_PAWN;
    game->board[0][0].type = game->board[0][7].type = CHESS_PIECE_TYPE_ROOK;
    game->board[0][1].type = game->board[0][6].type = CHESS_PIECE_TYPE_KNIGHT;
    game->board[0][2].type = game->board[0][5].type = CHESS_PIECE_TYPE_BISHOP;
    game->board[7][0].type = game->board[7][7].type = CHESS_PIECE_TYPE_ROOK;
    game->board[7][1].type = game->board[7][6].type = CHESS_PIECE_TYPE_KNIGHT;
    game->board[7][2].type = game->board[7][5].type = CHESS_PIECE_TYPE_BISHOP;
    game->board[0][3].type = game->board[7][3].type = CHESS_PIECE_TYPE_QUEEN;
    game->board[0][4].type = game->board[7][4].type = CHESS_PIECE_TYPE_KING;
    game->board[0][0].color = game->board[0][1].color = game->board[0][2].color = game->board[0][3].color =
    game->board[0][4].color = game->board[0][5].color = game->board[0][6].color = game->board[0][7].color = 
    game->board[1][0].color = game->board[1][1].color = game->board[1][2].color = game->board[1][3].color =
    game->board[1][4].color = game->board[1][5].color = game->board[1][6].color = game->board[1][7].color = CHESS_PLAYER_COLOR_WHITE;
    game->board[6][0].color = game->board[6][1].color = game->board[6][2].color = game->board[6][3].color =
    game->board[6][4].color = game->board[6][5].color = game->board[6][6].color = game->board[6][7].color = 
    game->board[7][0].color = game->board[7][1].color = game->board[7][2].color = game->board[7][3].color =
    game->board[7][4].color = game->board[7][5].color = game->board[7][6].color = game->board[7][7].color = CHESS_PLAYER_COLOR_BLACK;
}

ChessGame* ChessGame_Create() {
    ChessGame *game = malloc(sizeof(ChessGame));
    if (!game) return NULL;
    game->status = CHS_RUNNING;
    ChessGame_SetDefaultSettings(game);
    initChessBoard(game);
    game->currentTurn = CHESS_PLAYER_COLOR_WHITE;
    game->isWhiteKingThreatened = 0;
    game->isBlackKingThreatened = 0;
    game->history = FSAStack_Create(CHESS_GAME_HISTORY_SIZE, sizeof(ChessMove));
    if (!game->history) {
        ChessGame_Destroy(game);
        return NULL;
    }
    return game;
}

void ChessGame_Destroy(ChessGame *game) {
    if (!game) return;
    FSAStack_Destroy(game->history);
    free(game);
}

ChessGameResult ChessGame_SetDefaultSettings(ChessGame *game) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    game->settings.mode = CHESS_GAME_MODE_1_PLAYER;
    game->settings.difficulty = CHESS_GAME_DIFFICULTY_EASY;
    game->settings.userColor = CHESS_PLAYER_COLOR_WHITE;
    return CHESS_GAME_SUCCESS;
}

ChessGameResult ChessGame_SetGameMode(ChessGame *game, ChessGameMode mode) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    if (mode < CHESS_GAME_MODE_1_PLAYER) return CHESS_GAME_INVALID_ARGUMENT;
    if (mode > CHESS_GAME_MODE_2_PLAYER) return CHESS_GAME_INVALID_ARGUMENT;
    game->settings.mode = mode;
    return CHESS_GAME_SUCCESS;
}

ChessGameResult ChessGame_SetDifficulty(ChessGame *game, ChessGameDifficulty difficulty) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    if (difficulty < CHESS_GAME_DIFFICULTY_AMATEUR) return CHESS_GAME_INVALID_ARGUMENT;
    if (difficulty > CHESS_GAME_DIFFICULTY_EXPERT) return CHESS_GAME_INVALID_ARGUMENT;
    game->settings.difficulty = difficulty;
    return CHESS_GAME_SUCCESS;
}

ChessGameResult ChessGame_SetUserColor(ChessGame *game, ChessPlayerColor userColor) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    if (userColor < CHESS_PLAYER_COLOR_BLACK) return CHESS_GAME_INVALID_ARGUMENT;
    if (userColor > CHESS_PLAYER_COLOR_WHITE) return CHESS_GAME_INVALID_ARGUMENT;
    game->settings.userColor = userColor;
    return CHESS_GAME_SUCCESS;
}

int isValidPositionsOnBoard(ChessMove move) {
    return move.from.x >= 0 && move.from.x < CHESS_GAME_GRID &&
           move.from.y >= 0 && move.from.y < CHESS_GAME_GRID &&
           move.to.y >= 0 && move.to.y < CHESS_GAME_GRID &&
           move.to.y >= 0 && move.to.y < CHESS_GAME_GRID;
}

int isMoveOfPlayerPiece(ChessGame *game, ChessMove move) {
    if (!game) return 0; // sanity check
    return game->board[move.from.x][move.from.y].type != CHESS_PIECE_TYPE_NONE &&
           game->board[move.from.x][move.from.y].color == game->currentTurn;
}

/**
 * Check whether a given moving piece isn't stepping on a friendly piece.
 * A friendly piece is a piece that shares the same color as the moving piece.
 * @param   game        the game instance which provides the board
 * @param   move        the move to check it's legitimacy
 * @return              0 if the 'end' move location contains a friendly piece
 *                      1 otherwise
 */
int isValidToPosition(ChessGame *game, ChessMove move) {
    if (!game) return 0; // sanity check
    ChessPlayerColor fromColor = game->board[move.from.x][move.from.y].color;
    ChessPlayerColor toColor = game->board[move.to.x][move.to.y].color;
    return fromColor != toColor;
}

int isValidPawnMove(ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID], ChessMove move) {
    ChessPlayerColor color = board[move.from.x][move.from.y].color;
    int isInStartPos = move.from.y == (color == CHESS_PLAYER_COLOR_WHITE ? 1 : 6);
    int horDiff = abs(move.from.x - move.to.x);
    int verDiff = (move.from.y - move.to.y) * (color == CHESS_PLAYER_COLOR_WHITE ? 1 : -1);
    int isCapture = board[move.to.x][move.to.y].type != CHESS_PIECE_TYPE_NONE &&
        color != board[move.to.x][move.to.y].color;
    int regularMove = !isCapture && verDiff == 1 && horDiff == 0;
    int startingMove = !isCapture && isInStartPos && verDiff == 2 && horDiff == 0;
    int capturingMove = !isCapture && verDiff == 1 && horDiff == 1;
    return regularMove || startingMove || capturingMove;
}

int isValidRookMove(ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID], ChessMove move) {
    int horDiff = move.from.x - move.to.x;
    int verDiff = move.from.y - move.to.y;
    if ((horDiff != 0) ^ (verDiff != 0)) return 0; // exclusive ver / hor move
    if (horDiff != 0) { // check there isn't overleap
        int start = move.from.x < move.to.x ? move.from.x + 1 : move.to.x + 1;
        int end = move.from.x < move.to.x ? move.to.x : move.from.x;
        for (int i = start; i < end; i++) {
            if (board[i][move.from.y].type != CHESS_PIECE_TYPE_NONE) return 0;
        }
    } else { // verDiff != 0
        int start = move.from.y < move.to.y ? move.from.y + 1 : move.to.y + 1;
        int end = move.from.y < move.to.y ? move.to.y : move.from.y;
        for (int i = start + 1; i < end; i++) {
            if (board[move.from.x][i].type != CHESS_PIECE_TYPE_NONE) return 0;
        }
    }
    return 1;
}

int isValidKnightMove(ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID], ChessMove move) {
    (void)board; // TODO: consider remove that argument as it's unused.
    int horDiff = abs(move.from.x - move.to.x);
    int verDiff = abs(move.from.y - move.to.y);
    return (verDiff == 2 && horDiff == 1) ^ (horDiff == 2 && verDiff == 1);
}

int isValidBishopMove(ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID], ChessMove move) {
    int horAbs = abs(move.from.x - move.to.x);
    int verAbs = abs(move.from.y - move.to.y);
    if (horAbs != verAbs) return 0;
    int startX = move.from.x < move.to.x ? move.from.x + 1 : move.to.x + 1;
    int startY = move.from.y < move.to.y ? move.from.y + 1 : move.to.y + 1;
    int endX = startX == move.from.x + 1 ? move.to.x : move.from.x;
    int endY = startY == move.from.y + 1 ? move.to.y : move.from.y;
    for (int i = startX; i < endX; i++){
        for (int j = startY; j < endY; j++){
            if (board[i][j].type != CHESS_PIECE_TYPE_NONE) return 0;
        }
    }
    return 1;
}

int isValidQueenMove(ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID], ChessMove move) {
    return isValidRookMove(board, move) || isValidBishopMove(board, move);
}

int isValidKingMove(ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID], ChessMove move) {
    (void)board; // TODO: consider remove that argument as it's unused.
    int horDiff = abs(move.from.x - move.to.x);
    int verDiff = abs(move.from.y - move.to.y);
    return horDiff <= 1 && verDiff <= 1 && ((horDiff > 0) ^ (verDiff > 0));
}

int isValidPieceMove(ChessGame *game, ChessMove move) {
    if (!game) return 0;
    switch (game->board[move.from.x][move.from.y].type)
    {
        case CHESS_PIECE_TYPE_PAWN:
            return isValidPawnMove(game->board, move);
        case CHESS_PIECE_TYPE_ROOK:
            return isValidRookMove(game->board, move);    
        case CHESS_PIECE_TYPE_KNIGHT:
            return isValidKnightMove(game->board, move);
        case CHESS_PIECE_TYPE_BISHOP:
            return isValidBishopMove(game->board, move);
        case CHESS_PIECE_TYPE_QUEEN:
            return isValidQueenMove(game->board, move);
        case CHESS_PIECE_TYPE_KING:
            return isValidKingMove(game->board, move);
        case CHESS_PIECE_TYPE_NONE:
        default:
            return 0; // shouldn't happen
    }
}

int isKingThreatened(ChessGame *game) {
    ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID] = game->board;
    ChessPlayerColor playerColor = game->currentTurn;
    ChessPlayerColor opponentColor = 3 - playerColor;
    ChessMove move = { .to = getKingPosition(board, opponentColor) };
    for (int i = 0; i < CHESS_GAME_GRID; i++) {
        for (int j = 0; i < CHESS_GAME_GRID; i++) {
            if (board[i][j].color == playerColor) {
                move.from.x = i;
                move.from.y = j;
                if (isValidPieceMove(game, move)) return 0;
            }
        }
    }
    return 1;
}

void pseudoDoMove(ChessGame *game, ChessMove move) {
    if (!game) return; // sanity check
    move.capturedPiece = game->board[move.to.x][move.to.y];
    game->board[move.to.x][move.to.y] = game->board[move.from.x][move.from.y];
    game->board[move.from.x][move.from.y].type = CHESS_PIECE_TYPE_NONE;
    game->currentTurn = 3 - game->currentTurn; // elegant way to switch player
    FSAStack_Push(game->history, &move); // update history
    // TODO: update status, isWhiteKingThreatened & isBlackKingThreatened
}

ChessGameResult ChessGame_IsValidMove(ChessGame *game, ChessMove move) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    if (!isValidPositionsOnBoard(move)) return CHESS_GAME_INVALID_POSITION;
    if (!isMoveOfPlayerPiece(game, move)) return CHESS_GAME_EMPTY_POSITION;
    if (!isValidToPosition(game, move)) return CHESS_GAME_ILLEGAL_MOVE;
    if (!isValidPieceMove(game, move)) return CHESS_GAME_ILLEGAL_MOVE;
    // move specific piece
    // int isKingWillBeThreatened = isKingThretened(game->currentTurn);
    // get specific piece back in place
    // if (isKingWillBeThreatened) {
    //     return isKingThreatened(game) ?
    //         CHESS_GAME_KING_IS_STILL_THREATENED :
    //         CHESS_GAME_KING_WILL_BE_THREATENED;  
    // }
    return CHESS_GAME_SUCCESS;
}

ChessGameResult ChessGame_DoMove(ChessGame *game, ChessMove move) {
    ChessGameResult isValidResult = ChessGame_IsValidMove(game, move);
    if (isValidResult != CHESS_GAME_SUCCESS) return isValidResult;
    pseudoDoMove(game, move);
    return CHESS_GAME_SUCCESS;
}

ChessGameResult ChessGame_UndoMove(ChessGame *game) {
    ChessMove *move = FSAStack_Pop(game->history);
    game->board[move->from.x][move->from.y] = game->board[move->to.x][move->to.y];
    game->board[move->to.x][move->to.y] = move->capturedPiece; 
    return CHESS_GAME_SUCCESS;
}

ChessBoardPos getKingPosition(ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID], ChessPlayerColor color){
    for (int i = 0 ; i < CHESS_GAME_GRID; i++){
        for (int j = 0; j < CHESS_GAME_GRID; j++){
            if (board[i][j].type == CHESS_PIECE_TYPE_KING && board[i][j].color == color){
                ChessBoardPos pos = {.x = i, .y = j};
                return pos;
            }
        }
    }
}
// ============================================================================
// TODO: move to the right module

#define SETTINGS_STRING_1_PLAYER "SETTINGS:\nGAME_MODE: 1-player\nDIFFICULTY: %d\nUSER_COLOR: %s\n"
#define SETTINGS_STRING_2_PLAYER "SETTINGS:\nGAME_MODE: 2-player\n"

ChessGameResult ChessGame_PrintSettings(ChessGame *game) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    if (game->settings.mode == CHESS_GAME_MODE_2_PLAYER) {
        printf(SETTINGS_STRING_2_PLAYER);
    } else if (game->settings.mode == CHESS_GAME_MODE_1_PLAYER) {
        int difficulty = game->settings.difficulty + 1;
        char *userColor = game->settings.userColor == CHESS_PLAYER_COLOR_BLACK ? "black"  : "white";
        printf(SETTINGS_STRING_1_PLAYER, difficulty, userColor);
    }
    return CHESS_GAME_SUCCESS;
}
