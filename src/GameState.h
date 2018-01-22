#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include "ChessGame.h"


typedef enum GameErrorType_t {
    ERROR_INVALID_COMMAND,
    ERROR_INVALID_GAME_MODE,
    ERROR_INVALID_DIFF_LEVEL,
    ERROR_INVALID_USER_COLOR,
    ERROR_INVALID_FILE,
    ERROR_INVALID_POSITION,
    ERROR_EMPTY_POSITION,
    ERROR_INVALID_MOVE,
    ERROR_INVALID_MOVE_KING_IS_T,
    ERROR_INVALID_MOVE_KING_WILL_T,
    ERROR_FILE_ALLOC,
    ERROR_EMPTY_HISTORY,
} GameErrorType;

typedef struct GameError_t {
    GameErrorType errorType;
    char *description;
} GameError;

typedef enum GameStatestate_t {
    SETTINGS,
    RUNNING,
    PLAYER1_WON,
    PLAYER2_WON,
    TIE,
    ERROR,
} GameStateState;

typedef struct GameState_t {
    ChessGame *game;
    GameStateState state;
    GameError error;
} GameState;


#endif
