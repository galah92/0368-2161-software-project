#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include "ChessGame.h"


typedef enum GameErrorType_t {
    ERROR_INVALID_COMMAND,
    ERROR_WRONG_GAME_MODE,
    ERROR_WRONG_DIFF_LEVEL,
    ERROR_WRONG_USER_COLOR,
    ERROR_FILE_NOT_EXIST,
    ERROR_INVALID_POSITION,
    ERROR_POSITION_EMPTY,
    ERROR_ILLEGAL_MOVE,
    ERROR_ILLEGAL_MOVE_KING_IS_T,
    ERROR_ILLEGAL_MOVE_KING_WILL_T,
    ERROR_FILE_CANNOT_BE_CREATED,
    ERROR_EMPTY_HISTORY,
    ERROR_CUSTOME,
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
