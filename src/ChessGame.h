#ifndef CHESS_GAME_H_
#define CHESS_GAME_H_

#include "FSAStack.h"

#define CHESS_GAME_MODE_1_PLAYER 1
#define CHESS_GAME_MODE_2_PLAYER 2
#define CHESS_GAME_USER_COLOR_BLACK 1
#define CHESS_GAME_USER_COLOR_WHITE 2
#define CHESS_GAME_BOARD_SIZE 8


typedef enum {
    CHESS_GAME_SUCCESS,
    CHESS_GAME_INVALID_ARGUMENT,
    CHESS_GAME_INVALID_POSITION,
    CHESS_GAME_EMPTY_POSITION,
    CHESS_GAME_ILLEGAL_MOVE,
    CHESS_GAME_KING_IS_STILL_THREATENED,
    CHESS_GAME_KING_WILL_BE_THREATENED,

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
    FSAStack *history;
} ChessGame;

typedef struct {
    unsigned char x;
    unsigned char y;
} ChessBoardPos;

typedef struct {
    ChessBoardPos from;
    ChessBoardPos to;
} ChessMove;


ChessGame* ChessGame_Create();

void ChessGame_Destroy(ChessGame *game);

CHESS_GAME_RESULT ChessGame_SetDefaultSettings(ChessGame *game);

CHESS_GAME_RESULT ChessGame_SetGameMode(ChessGame *game, int mode);

CHESS_GAME_RESULT ChessGame_SetDifficulty(ChessGame *game, int difficulty);

CHESS_GAME_RESULT ChessGame_SetUserColor(ChessGame *game, int userColor);

CHESS_GAME_RESULT ChessGame_PrintSettings(ChessGame *game);

CHESS_GAME_RESULT ChessGame_GetSettingsString(ChessGame *game, char *outString);

CHESS_GAME_RESULT ChessGame_IsLegalMove(ChessGame *game, ChessMove move);

CHESS_GAME_RESULT ChessGame_DoMove(ChessGame *game, ChessMove move);

CHESS_GAME_RESULT ChessGame_GetMoves(ChessGame *game, ChessBoardPos pos, ChessMove *moves[]);


#endif
