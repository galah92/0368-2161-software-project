#ifndef CHESS_GAME_H_
#define CHESS_GAME_H_

#define CHESS_GAME_MODE_1_PLAYER 1
#define CHESS_GAME_MODE_2_PLAYER 2
#define CHESS_GAME_USER_COLOR_BLACK 1
#define CHESS_GAME_USER_COLOR_WHITE 2
#define CHESS_GAME_BOARD_SIZE 8


typedef enum {
    CHESS_GAME_SUCCESS,
    CHESS_GAME_INVALID_ARGUMENT,
} CHESS_GAME_RESULT;

typedef struct {
    int mode;
    int difficulty;
    int userColor;
} ChessGameSettings;

typedef struct {
    ChessGameSettings settings;
    char board[CHESS_GAME_BOARD_SIZE][CHESS_GAME_BOARD_SIZE];
    int currentPlayer;
} ChessGame;


ChessGame* ChessGame_Create();

void ChessGame_Destroy(ChessGame *game);

CHESS_GAME_RESULT ChessGame_SetDefaultSettings(ChessGame *game);

CHESS_GAME_RESULT ChessGame_SetGameMode(ChessGame *game, int mode);

CHESS_GAME_RESULT ChessGame_SetDifficulty(ChessGame *game, int difficulty);

CHESS_GAME_RESULT ChessGame_SetUserColor(ChessGame *game, int userColor);

CHESS_GAME_RESULT ChessGame_PrintSettings(ChessGame *game);

CHESS_GAME_RESULT ChessGame_GetSettingsString(ChessGame *game, char *outString);


#endif
