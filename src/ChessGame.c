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

int isValidPawnMove(ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID], ChessMove move) {
    ChessPlayerColor color = board[move.from.x][move.from.y].color;
    int isInStartPos = move.from.y == (color == CHESS_PLAYER_COLOR_WHITE ? 1 : 6);
    int horDiff = (move.from.x - move.to.x) * (color == CHESS_PLAYER_COLOR_WHITE ? 1 : -1);
    int verDiff = (move.from.y - move.to.y) * (color == CHESS_PLAYER_COLOR_WHITE ? 1 : -1);
    int isCapture = board[move.to.x][move.to.y].type != CHESS_PIECE_TYPE_NONE &&
        color != board[move.to.x][move.to.y].color;
    int regularMove = !isCapture && verDiff == 1 && horDiff == 0;
    int startingMove = !isCapture && isInStartPos && verDiff == 2 && horDiff == 0;
    int capturingMove = !isCapture && verDiff == 1 && horDiff == 1;
    return regularMove || startingMove || capturingMove;
}

int isValidRookMove(ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID], ChessMove move) {
    (void)board;
    (void)move;
    return 0;
}

int isValidKnightMove(ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID], ChessMove move) {
    (void)board;
    (void)move;
    return 0;
}

int isValidBishopMove(ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID], ChessMove move) {
    (void)board;
    (void)move;
    return 0;
}

int isValidQueenMove(ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID], ChessMove move) {
    (void)board;
    (void)move;
    return 0;
}

int isValidKingMove(ChessTile board[CHESS_GAME_GRID][CHESS_GAME_GRID], ChessMove move) {
    (void)board;
    (void)move;
    return 0;
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
