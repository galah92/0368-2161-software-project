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

typedef enum GameError {
    GAME_ERROR_NONE,
    GAME_ERROR_INVALID_COMMAND,
    GAME_ERROR_INVALID_GAME_MODE,
    GAME_ERROR_INVALID_DIFF_LEVEL,
    GAME_ERROR_INVALID_USER_COLOR,
    GAME_ERROR_INVALID_FILE,
    GAME_ERROR_INVALID_POSITION,
    GAME_ERROR_EMPTY_POSITION,
    GAME_ERROR_INVALID_MOVE,
    GAME_ERROR_INVALID_MOVE_KING_IS_T,
    GAME_ERROR_INVALID_MOVE_KING_WILL_T,
    GAME_ERROR_FILE_ALLOC,
    GAME_ERROR_EMPTY_HISTORY,
} GameError;

typedef enum GamePhase {
    GAME_PHASE_SETTINGS,
    GAME_PHASE_RUNNING,
    GAME_PHASE_ERROR,
    GAME_PHASE_QUIT,
} GamePhase;

typedef struct GameState {
    ChessGame *game;
    GamePhase state;
    GameError error;
} GameState;

typedef enum GamePlayerType {
    GAME_PLAYER_TYPE_HUMAN,
    GAME_PLAYER_TYPE_AI
} GamePlayerType;

typedef struct GameManager {
    ChessGame *game;
    GamePhase phase;
    GameError error;
} GameManager;

GameManager* GameManager_Create();

void GameManager_Destroy(GameManager *manager);

GamePlayerType GameManager_GetCurrentPlayerType(GameManager *manager);

void GameManager_ProcessCommand(GameManager *manager, GameCommand command);


#endif
