#include <stdio.h>
#include <stdlib.h>
#include "ChessGame.h"

#define SETTINGS_STRING_1_PLAYER "SETTINGS:\nGAME_MODE: 1-player\nDIFFICULTY: %d\nUSER_COLOR: %s\n"
#define SETTINGS_STRING_2_PLAYER "SETTINGS:\nGAME_MODE: 2-player\n"


ChessGame* ChessGame_Create() {
    ChessGame *game = malloc(sizeof(ChessGame));
    if (!game) return NULL;
    ChessGame_SetDefaultSettings(game);
    return game;
}

void ChessGame_Destroy(ChessGame *game) {
    free(game);
}

CHESS_GAME_RESULT ChessGame_SetDefaultSettings(ChessGame *game) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    game->settings.mode = CHESS_GAME_MODE_1_PLAYER;
    game->settings.difficulty = 2;
    game->settings.userColor = CHESS_GAME_USER_COLOR_WHITE;
    return CHESS_GAME_SUCCESS;
}

CHESS_GAME_RESULT ChessGame_SetGameMode(ChessGame *game, int mode) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    game->settings.mode = mode;
    return CHESS_GAME_SUCCESS;
}

CHESS_GAME_RESULT ChessGame_SetDifficulty(ChessGame *game, int difficulty) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    game->settings.difficulty = difficulty;
    return CHESS_GAME_SUCCESS;
}

CHESS_GAME_RESULT ChessGame_SetUserColor(ChessGame *game, int userColor) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    game->settings.userColor = userColor;
    return CHESS_GAME_SUCCESS;
}

CHESS_GAME_RESULT ChessGame_PrintSettings(ChessGame *game) {
    if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    if (game->settings.mode == CHESS_GAME_MODE_2_PLAYER) {
        printf(SETTINGS_STRING_2_PLAYER);
    } else if (game->settings.mode == CHESS_GAME_MODE_1_PLAYER) {
        int difficulty = game->settings.difficulty + 1;
        char *userColor = game->settings.userColor == CHESS_GAME_USER_COLOR_BLACK ? "black"  : "white";
        printf(SETTINGS_STRING_1_PLAYER, difficulty, userColor);
    }
    return CHESS_GAME_SUCCESS;
}

CHESS_GAME_RESULT ChessGame_GetSettingsString(ChessGame *game, char *outString) {
        if (!game) return CHESS_GAME_INVALID_ARGUMENT;
    if (game->settings.mode == CHESS_GAME_MODE_2_PLAYER) {
        sprintf(outString, SETTINGS_STRING_2_PLAYER);
    } else if (game->settings.mode == CHESS_GAME_MODE_1_PLAYER) {
        int difficulty = game->settings.difficulty + 1;
        char *userColor = game->settings.userColor == CHESS_GAME_USER_COLOR_BLACK ? "black"  : "white";
        sprintf(outString, SETTINGS_STRING_1_PLAYER, difficulty, userColor);
    }
    return CHESS_GAME_SUCCESS;
}
