#include <stdio.h>
#include <stdlib.h>
#include "ChessGame.h"


void initChessBoard(ChessGame *game) {
    game->board[2][0].piece = game->board[2][1].piece = game->board[2][2].piece = game->board[2][3].piece =
    game->board[2][4].piece = game->board[2][5].piece = game->board[2][6].piece = game->board[2][7].piece =
    game->board[3][0].piece = game->board[3][1].piece = game->board[3][2].piece = game->board[3][3].piece =
    game->board[3][4].piece = game->board[3][5].piece = game->board[3][6].piece = game->board[3][7].piece =
    game->board[4][0].piece = game->board[4][1].piece = game->board[4][2].piece = game->board[4][3].piece =
    game->board[4][4].piece = game->board[4][5].piece = game->board[4][6].piece = game->board[4][7].piece =
    game->board[5][0].piece = game->board[5][1].piece = game->board[5][2].piece = game->board[5][3].piece =
    game->board[5][4].piece = game->board[5][5].piece = game->board[5][6].piece = game->board[5][7].piece = CHESS_PIECE_NONE;
    game->board[1][0].piece = game->board[1][1].piece = game->board[1][2].piece = game->board[1][3].piece =
    game->board[1][4].piece = game->board[1][5].piece = game->board[1][6].piece = game->board[1][7].piece = CHESS_PIECE_PAWN;
    game->board[6][0].piece = game->board[6][1].piece = game->board[6][2].piece = game->board[6][3].piece =
    game->board[6][4].piece = game->board[6][5].piece = game->board[6][6].piece = game->board[6][7].piece = CHESS_PIECE_PAWN;
    game->board[0][0].piece = game->board[0][7].piece = CHESS_PIECE_ROOK;
    game->board[0][1].piece = game->board[0][6].piece = CHESS_PIECE_KNIGHT;
    game->board[0][2].piece = game->board[0][5].piece = CHESS_PIECE_BISHOP;
    game->board[7][0].piece = game->board[7][7].piece = CHESS_PIECE_ROOK;
    game->board[7][1].piece = game->board[7][6].piece = CHESS_PIECE_KNIGHT;
    game->board[7][2].piece = game->board[7][5].piece = CHESS_PIECE_BISHOP;
    game->board[0][3].piece = game->board[7][3].piece = CHESS_PIECE_QUEEN;
    game->board[0][4].piece = game->board[7][4].piece = CHESS_PIECE_KING;
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
    game->status = CHESS_STATUS_RUNNING;
    ChessGame_SetDefaultSettings(game);
    initChessBoard(game);
    game->turn = CHESS_PLAYER_COLOR_WHITE;
    game->history = ArrayStack_Create(CHESS_HISTORY_SIZE, sizeof(ChessMove));
    if (!game->history) {
        ChessGame_Destroy(game);
        return NULL;
    }
    return game;
}

void ChessGame_Destroy(ChessGame *game) {
    if (!game) return;
    ArrayStack_Destroy(game->history);
    free(game);
}

ChessResult ChessGame_SetDefaultSettings(ChessGame *game) {
    if (!game) return CHESS_INVALID_ARGUMENT;
    game->settings.mode = CHESS_MODE_1_PLAYER;
    game->settings.difficulty = CHESS_DIFFICULTY_EASY;
    game->settings.userColor = CHESS_PLAYER_COLOR_WHITE;
    return CHESS_SUCCESS;
}

ChessResult ChessGame_SetGameMode(ChessGame *game, ChessMode mode) {
    if (!game) return CHESS_INVALID_ARGUMENT;
    if (mode < CHESS_MODE_1_PLAYER) return CHESS_INVALID_ARGUMENT;
    if (mode > CHESS_MODE_2_PLAYER) return CHESS_INVALID_ARGUMENT;
    game->settings.mode = mode;
    return CHESS_SUCCESS;
}

ChessResult ChessGame_SetDifficulty(ChessGame *game, ChessDifficulty difficulty) {
    if (!game) return CHESS_INVALID_ARGUMENT;
    if (difficulty < CHESS_DIFFICULTY_AMATEUR) return CHESS_INVALID_ARGUMENT;
    if (difficulty > CHESS_DIFFICULTY_EXPERT) return CHESS_INVALID_ARGUMENT;
    game->settings.difficulty = difficulty;
    return CHESS_SUCCESS;
}

ChessResult ChessGame_SetUserColor(ChessGame *game, ChessColor userColor) {
    if (!game) return CHESS_INVALID_ARGUMENT;
    if (userColor < CHESS_PLAYER_COLOR_BLACK) return CHESS_INVALID_ARGUMENT;
    if (userColor > CHESS_PLAYER_COLOR_WHITE) return CHESS_INVALID_ARGUMENT;
    game->settings.userColor = userColor;
    return CHESS_SUCCESS;
}

int isValidPositionsOnBoard(ChessMove move) {
    return move.from.x >= 0 && move.from.x < CHESS_GRID &&
           move.from.y >= 0 && move.from.y < CHESS_GRID &&
           move.to.y >= 0 && move.to.y < CHESS_GRID &&
           move.to.y >= 0 && move.to.y < CHESS_GRID;
}

int isMoveOfPlayerPiece(ChessGame *game, ChessMove move) {
    if (!game) return 0; // sanity check
    return game->board[move.from.x][move.from.y].piece != CHESS_PIECE_NONE &&
           game->board[move.from.x][move.from.y].color == game->turn;
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
    ChessColor fromColor = game->board[move.from.x][move.from.y].color;
    ChessColor toColor = game->board[move.to.x][move.to.y].color;
    return fromColor != toColor;
}

int isValidPawnMove(ChessGame *game, ChessMove move) {
    ChessColor color = game->board[move.from.x][move.from.y].color;
    int isInStartPos = move.from.y == (color == CHESS_PLAYER_COLOR_WHITE ? 1 : 6);
    int horDiff = abs(move.from.x - move.to.x);
    int verDiff = (move.from.y - move.to.y) * (color == CHESS_PLAYER_COLOR_WHITE ? 1 : -1);
    int isCapture = game->board[move.to.x][move.to.y].piece != CHESS_PIECE_NONE &&
        color != game->board[move.to.x][move.to.y].color;
    int regularMove = !isCapture && verDiff == 1 && horDiff == 0;
    int startingMove = !isCapture && isInStartPos && verDiff == 2 && horDiff == 0;
    int capturingMove = !isCapture && verDiff == 1 && horDiff == 1;
    return regularMove || startingMove || capturingMove;
}

int isValidRookMove(ChessGame *game, ChessMove move) {
    int horDiff = move.from.x - move.to.x;
    int verDiff = move.from.y - move.to.y;
    if ((horDiff != 0) ^ (verDiff != 0)) return 0; // exclusive ver / hor move
    if (horDiff != 0) { // check there isn't overleap
        int start = move.from.x < move.to.x ? move.from.x + 1 : move.to.x + 1;
        int end = move.from.x < move.to.x ? move.to.x : move.from.x;
        for (int i = start; i < end; i++) {
            if (game->board[i][move.from.y].piece != CHESS_PIECE_NONE) return 0;
        }
    } else { // verDiff != 0
        int start = move.from.y < move.to.y ? move.from.y + 1 : move.to.y + 1;
        int end = move.from.y < move.to.y ? move.to.y : move.from.y;
        for (int i = start + 1; i < end; i++) {
            if (game->board[move.from.x][i].piece != CHESS_PIECE_NONE) return 0;
        }
    }
    return 1;
}

int isValidKnightMove(ChessGame *game, ChessMove move) {
    (void)game; // TODO: consider remove that argument as it's unused.
    int horDiff = abs(move.from.x - move.to.x);
    int verDiff = abs(move.from.y - move.to.y);
    return (verDiff == 2 && horDiff == 1) ^ (horDiff == 2 && verDiff == 1);
}

int isValidBishopMove(ChessGame *game, ChessMove move) {
    int horAbs = abs(move.from.x - move.to.x);
    int verAbs = abs(move.from.y - move.to.y);
    if (horAbs != verAbs) return 0;
    int startX = move.from.x < move.to.x ? move.from.x + 1 : move.to.x + 1;
    int startY = move.from.y < move.to.y ? move.from.y + 1 : move.to.y + 1;
    int endX = startX == move.from.x + 1 ? move.to.x : move.from.x;
    int endY = startY == move.from.y + 1 ? move.to.y : move.from.y;
    for (int i = startX; i < endX; i++){
        for (int j = startY; j < endY; j++){
            if (game->board[i][j].piece != CHESS_PIECE_NONE) return 0;
        }
    }
    return 1;
}

int isValidQueenMove(ChessGame *game, ChessMove move) {
    return isValidRookMove(game, move) || isValidBishopMove(game, move);
}

int isValidKingMove(ChessGame *game, ChessMove move) {
    (void)game; // TODO: consider remove that argument as it's unused.
    int horDiff = abs(move.from.x - move.to.x);
    int verDiff = abs(move.from.y - move.to.y);
    return horDiff <= 1 && verDiff <= 1 && ((horDiff > 0) ^ (verDiff > 0));
}

int isValidPieceMove(ChessGame *game, ChessMove move) {
    if (!game) return 0;
    switch (game->board[move.from.x][move.from.y].piece)
    {
        case CHESS_PIECE_PAWN:
            return isValidPawnMove(game, move);
        case CHESS_PIECE_ROOK:
            return isValidRookMove(game, move);    
        case CHESS_PIECE_KNIGHT:
            return isValidKnightMove(game, move);
        case CHESS_PIECE_BISHOP:
            return isValidBishopMove(game, move);
        case CHESS_PIECE_QUEEN:
            return isValidQueenMove(game, move);
        case CHESS_PIECE_KING:
            return isValidKingMove(game, move);
        case CHESS_PIECE_NONE:
        default:
            return 0; // shouldn't happen
    }
}

ChessBoardPos getKingPosition(ChessGame *game, ChessColor color){
    for (int i = 0 ; i < CHESS_GRID; i++) {
        for (int j = 0; j < CHESS_GRID; j++) {
            if (game->board[i][j].piece == CHESS_PIECE_KING &&
                game->board[i][j].color == color) {
                return (ChessBoardPos){ .x = i, .y = j };
            }
        }
    }
    return (ChessBoardPos){ .x = -1, .y = -1 }; // can't happen
}

int isKingThreatened(ChessGame *game, ChessColor playerColor) {
    ChessColor opponentColor = 3 - playerColor;
    ChessMove move = { .to = getKingPosition(game, opponentColor) };
    for (int i = 0; i < CHESS_GRID; i++) {
        for (int j = 0; i < CHESS_GRID; i++) {
            if (game->board[i][j].color == playerColor) {
                move.from.x = i;
                move.from.y = j;
                if (isValidPieceMove(game, move)) return 1;
            }
        }
    }
    return 0;
}

void pseudoDoMove(ChessGame *game, ChessMove move) {
    if (!game) return; // sanity check
    move.capturedPiece = game->board[move.to.x][move.to.y];
    game->board[move.to.x][move.to.y] = game->board[move.from.x][move.from.y];
    game->board[move.from.x][move.from.y].piece = CHESS_PIECE_NONE;
    game->turn = 3 - game->turn; // elegant way to switch player
    ArrayStack_Push(game->history, &move); // update history
    if (isKingThreatened(game, game->turn)) {
        game->status = CHESS_STATUS_CHECK;
        // TODO: check if opponent has a way to escape, else change
        // status to CHESS_STATUS_CHECKMATE
    }
}

ChessResult ChessGame_IsValidMove(ChessGame *game, ChessMove move) {
    if (!game) return CHESS_INVALID_ARGUMENT;
    if (!isValidPositionsOnBoard(move)) return CHESS_INVALID_POSITION;
    if (!isMoveOfPlayerPiece(game, move)) return CHESS_EMPTY_POSITION;
    if (!isValidToPosition(game, move)) return CHESS_ILLEGAL_MOVE;
    if (!isValidPieceMove(game, move)) return CHESS_ILLEGAL_MOVE;
    pseudoDoMove(game, move);
    int isKingWillBeThreatened = isKingThreatened(game, !game->turn);
    ChessGame_UndoMove(game);
    if (game->status == CHESS_STATUS_CHECK && isKingWillBeThreatened) {
        return CHESS_KING_IS_STILL_THREATENED; 
    } else if (isKingWillBeThreatened) {
        return CHESS_KING_WILL_BE_THREATENED;
    }
    return CHESS_SUCCESS;
}

ChessResult ChessGame_DoMove(ChessGame *game, ChessMove move) {
    ChessResult isValidResult = ChessGame_IsValidMove(game, move);
    if (isValidResult != CHESS_SUCCESS) return isValidResult;
    pseudoDoMove(game, move);
    return CHESS_SUCCESS;
}

ChessResult ChessGame_UndoMove(ChessGame *game) {
    ChessMove *move = ArrayStack_Pop(game->history);
    game->board[move->from.x][move->from.y] = game->board[move->to.x][move->to.y];
    game->board[move->to.x][move->to.y] = move->capturedPiece; 
    return CHESS_SUCCESS;
}

// ============================================================================
// TODO: move to the right module

#define SETTINGS_STRING_1_PLAYER "SETTINGS:\nGAME_MODE: 1-player\nDIFFICULTY: %d\nUSER_COLOR: %s\n"
#define SETTINGS_STRING_2_PLAYER "SETTINGS:\nGAME_MODE: 2-player\n"

ChessResult ChessGame_PrintSettings(ChessGame *game) {
    if (!game) return CHESS_INVALID_ARGUMENT;
    if (game->settings.mode == CHESS_MODE_2_PLAYER) {
        printf(SETTINGS_STRING_2_PLAYER);
    } else if (game->settings.mode == CHESS_MODE_1_PLAYER) {
        int difficulty = game->settings.difficulty + 1;
        char *userColor = game->settings.userColor == CHESS_PLAYER_COLOR_BLACK ? "black"  : "white";
        printf(SETTINGS_STRING_1_PLAYER, difficulty, userColor);
    }
    return CHESS_SUCCESS;
}
