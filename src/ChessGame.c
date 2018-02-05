#include <stdio.h>
#include <stdlib.h>
#include "ChessGame.h"


#define SETTINGS_STRING_1_PLAYER "SETTINGS:\nGAME_MODE: 1-player\nDIFFICULTY: %d\nUSER_COLOR: %s\n"
#define SETTINGS_STRING_2_PLAYER "SETTINGS:\nGAME_MODE: 2-player\n"
#define HISTORY_SIZE 3

void initChessBoard(ChessGame *game) {
    game->board[0][0].type = game->board[0][7].type = CHESS_PIECE_TYPE_ROOK;
    game->board[0][1].type = game->board[0][6].type = CHESS_PIECE_TYPE_KNIGHT;
    game->board[0][2].type = game->board[0][5].type = CHESS_PIECE_TYPE_BISHOP;
    game->board[7][0].type = game->board[7][7].type = CHESS_PIECE_TYPE_ROOK;
    game->board[7][1].type = game->board[7][6].type = CHESS_PIECE_TYPE_KNIGHT;
    game->board[7][2].type = game->board[7][5].type = CHESS_PIECE_TYPE_BISHOP;
    game->board[0][3].type = game->board[7][3].type = CHESS_PIECE_TYPE_QUEEN;
    game->board[0][4].type = game->board[7][4].type = CHESS_PIECE_TYPE_KING;
    game->board[0][0].color = game->board[0][7].color =
    game->board[0][1].color = game->board[0][6].color =
    game->board[0][2].color = game->board[0][5].color =
    game->board[0][3].color = game->board[0][4].color = CHESS_PLAYER_COLOR_WHITE;
    game->board[1][0].color = game->board[1][7].color =
    game->board[1][1].color = game->board[1][6].color =
    game->board[1][2].color = game->board[1][5].color =
    game->board[1][3].color = game->board[1][4].color = CHESS_PLAYER_COLOR_BLACK;
}

ChessGame* ChessGame_Create() {
    ChessGame *game = malloc(sizeof(ChessGame));
    if (!game) return NULL;
    ChessGame_SetDefaultSettings(game);
    initChessBoard(game);
    game->history = FSAStack_Create(HISTORY_SIZE, sizeof(ChessMove));
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

ChessGameResult ChessGame_SetGameMode(ChessGame *game, int mode) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    if (mode < CHESS_GAME_MODE_1_PLAYER) return CHESS_GAME_INVALID_ARGUMENT;
    if (mode > CHESS_GAME_MODE_2_PLAYER) return CHESS_GAME_INVALID_ARGUMENT;
    game->settings.mode = mode;
    return CHESS_GAME_SUCCESS;
}

ChessGameResult ChessGame_SetDifficulty(ChessGame *game, int difficulty) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    if (difficulty < CHESS_GAME_DIFFICULTY_AMATEUR) return CHESS_GAME_INVALID_ARGUMENT;
    if (difficulty > CHESS_GAME_DIFFICULTY_EXPERT) return CHESS_GAME_INVALID_ARGUMENT;
    game->settings.difficulty = difficulty;
    return CHESS_GAME_SUCCESS;
}

ChessGameResult ChessGame_SetUserColor(ChessGame *game, int userColor) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    if (userColor < CHESS_PLAYER_COLOR_BLACK) return CHESS_GAME_INVALID_ARGUMENT;
    if (userColor > CHESS_PLAYER_COLOR_WHITE) return CHESS_GAME_INVALID_ARGUMENT;
    game->settings.userColor = userColor;
    return CHESS_GAME_SUCCESS;
}

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

int isValidBoardPosition(ChessBoardPos pos) {
    return pos.x >= 0 && pos.x < CHESS_GAME_BOARD_SIZE && 
           pos.y >= 0 && pos.y < CHESS_GAME_BOARD_SIZE;
}

ChessGameResult ChessGame_IsValidMove(ChessGame *game, ChessMove move) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    if (!isValidBoardPosition(move.from)) return CHESS_GAME_INVALID_POSITION;
    if (!isValidBoardPosition(move.to)) return CHESS_GAME_INVALID_POSITION;
    ChessPlayerColor playerColor = game->currentPlayer;
    if (game->board[move.from.x][move.from.y].type == CHESS_PIECE_TYPE_NONE) return CHESS_GAME_EMPTY_POSITION;
    if (game->board[move.from.x][move.from.y].color != playerColor) return CHESS_GAME_INVALID_POSITION;
    // TODO: check that king is not threatened if move is made
    return CHESS_GAME_SUCCESS;
}

ChessGameResult ChessGame_DoMove(ChessGame *game, ChessMove move) {
    ChessGameResult isValidResult = ChessGame_IsValidMove(game, move);
    if (isValidResult != CHESS_GAME_SUCCESS) return isValidResult;
    game->board[move.to.x][move.to.y].type = game->board[move.from.x][move.from.y].type;
    game->board[move.to.x][move.to.y].color = game->board[move.from.x][move.from.y].color;
    game->board[move.from.x][move.from.y].type = CHESS_PIECE_TYPE_NONE;
    game->currentPlayer = 3 - game->currentPlayer;  // elegant way to switch player
    FSAStack_Push(game->history, &move);
    return CHESS_GAME_SUCCESS;
}
