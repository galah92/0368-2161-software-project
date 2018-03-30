#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "ChessGame.h"

#define GAME_COMMAND_MAX_LINE_LENGTH    1024
#define GAME_COMMAND_ARGS_CAPACITY      8


typedef enum GameCommandType {
    // settings-only commands
	GAME_COMMAND_GAME_MODE,
	GAME_COMMAND_DIFFICULTY,
	GAME_COMMAND_USER_COLOR,
    GAME_COMMAND_LOAD_GAME,
    GAME_COMMAND_DEFAULT_SETTINGS,
    GAME_COMMAND_PRINT_SETTINGS,
    GAME_COMMAND_START,
    // game-only commands
    GAME_COMMAND_MOVE,
    GAME_COMMAND_GET_MOVES,
    GAME_COMMAND_SAVE,
    GAME_COMMAND_UNDO,
    GAME_COMMAND_RESET,
    // shared commands
    GAME_COMMAND_QUIT,
    GAME_COMMAND_INVALID,
} GameCommandType;


typedef struct GameCommand {
	GameCommandType type;
	int args[GAME_COMMAND_ARGS_CAPACITY];
    char path[GAME_COMMAND_MAX_LINE_LENGTH];
} GameCommand;

typedef struct GameManager {
    ChessGame *game;
} GameMananger;

typedef enum GameErrorType {
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

typedef struct GameError {
    GameErrorType errorType;
    char *description;
} GameError;

typedef enum GameStatestate {
    SETTINGS,
    RUNNING,
    PLAYER1_WON,
    PLAYER2_WON,
    TIE,
    ERROR,
} GameStateState;

typedef struct GameState {
    ChessGame *game;
    GameStateState state;
    GameError error;
} GameState;

void GameManager_ProcessCommand(GameCommand command);


#endif
