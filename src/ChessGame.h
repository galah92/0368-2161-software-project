#ifndef CHESS_GAME_H_
#define CHESS_GAME_H_

#include "FSAStack.h"

#define CHESS_GAME_MODE_1_PLAYER        1
#define CHESS_GAME_MODE_2_PLAYER        2
#define CHESS_GAME_USER_COLOR_BLACK     1
#define CHESS_GAME_USER_COLOR_WHITE     2
#define CHESS_GAME_BOARD_SIZE           8


typedef enum ChessGameResult_t {
    CHESS_GAME_SUCCESS,
    CHESS_GAME_INVALID_ARGUMENT,
    CHESS_GAME_INVALID_POSITION,
    CHESS_GAME_EMPTY_POSITION,
    CHESS_GAME_ILLEGAL_MOVE,
    CHESS_GAME_KING_IS_STILL_THREATENED,
    CHESS_GAME_KING_WILL_BE_THREATENED,
} ChessGameResult;

typedef struct ChessGameSettings_t {
    int mode;
    int difficulty;
    int userColor;
} ChessGameSettings;

typedef enum ChessGameStatus_t {
    CHS_RUNNING,
    CHS_PLAYER1_WINNER,
    CHS_PLAYER2_WINNER,
    CHS_TIE,
} ChessGameStatus;

typedef struct ChessGame_t {
    ChessGameStatus status;
    ChessGameSettings settings;
    char board[CHESS_GAME_BOARD_SIZE][CHESS_GAME_BOARD_SIZE];
    int currentPlayer;
    FSAStack *history;
} ChessGame;

typedef struct ChessBoardPos_t {
    unsigned char x;
    unsigned char y;
} ChessBoardPos;

typedef struct ChessMove_t {
    ChessBoardPos from;
    ChessBoardPos to;
} ChessMove;


ChessGame* ChessGame_Create();

void ChessGame_Destroy(ChessGame *game);

ChessGameResult ChessGame_SetDefaultSettings(ChessGame *game);

ChessGameResult ChessGame_SetGameMode(ChessGame *game, int mode);

ChessGameResult ChessGame_SetDifficulty(ChessGame *game, int difficulty);

ChessGameResult ChessGame_SetUserColor(ChessGame *game, int userColor);

ChessGameResult ChessGame_PrintSettings(ChessGame *game);

ChessGameResult ChessGame_GetSettingsString(ChessGame *game, char *outString);

ChessGameResult ChessGame_IsLegalMove(ChessGame *game, ChessMove move);

ChessGameResult ChessGame_DoMove(ChessGame *game, ChessMove move);

ChessGameResult ChessGame_GetMoves(ChessGame *game, ChessBoardPos pos, ChessMove *moves[]);


#endif
