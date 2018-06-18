#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include <stdbool.h>
#include <stdio.h>
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
    GAME_COMMAND_RESTART,
    // shared commands
    GAME_COMMAND_QUIT,
    GAME_COMMAND_INVALID,
    // GUI commands
    GAME_COMMAND_LOAD_AND_START,
    GAME_COMMAND_SAVE_FROM_SLOT,
    GAME_COMMAND_SET_PANE,
    GAME_COMMAND_BACK_PANE,
    GAME_COMMAND_SET_SLOT,
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
    GAME_ERROR_NOT_CONTAIN_PLAYER_PIECE,
    GAME_ERROR_INVALID_MOVE,
    GAME_ERROR_INVALID_MOVE_KING_IS_T,
    GAME_ERROR_INVALID_MOVE_KING_WILL_T,
    GAME_ERROR_FILE_ALLOC,
    GAME_ERROR_EMPTY_HISTORY,
} GameError;

typedef enum GameStatus {
    GAME_STATUS_RUNNING,
    GAME_STATUS_CHECK,
    GAME_STATUS_CHECKMATE,
    GAME_STATUS_DRAW,
} GameStatus;

typedef enum GamePhase {
    GAME_PHASE_SETTINGS,
    GAME_PHASE_RUNNING,
    GAME_PHASE_ERROR,
    GAME_PHASE_QUIT,
} GamePhase;

typedef enum GamePlayerType {
    GAME_PLAYER_TYPE_HUMAN,
    GAME_PLAYER_TYPE_AI
} GamePlayerType;

typedef enum GamePaneType {
    GAME_PANE_TYPE_MAIN,
    GAME_PANE_TYPE_SETTINGS,
    GAME_PANE_TYPE_GAME,
    GAME_PANE_TYPE_LOAD,
} GamePaneType;

typedef struct GameManager {
    ChessGame *game;
    GamePhase phase;
    GameError error;
    ArrayStack *moves;
    GameStatus status;
    // GUI-related fields
    bool isSaved;
    unsigned int slot;
    GamePaneType paneType;
    GamePaneType lastPaneType;
} GameManager;

/**
 * Create new GameManager instance.
 * @return  NULL if malloc failed
 *          GameManager* instance otherwise
 */
GameManager* GameManager_Create();

/**
 * Free all resources for a given GameManager instance.
 * @param   manager     the instance to destroy
 * @return  NULL
 */
GameManager* GameManager_Destroy(GameManager *manager);

/**
 * Retrieve a given GameManager instance's current player type
 * @param   manager     the instance to work on
 * @return  -1          if manager == NULL
 *          GamePlayerType otherwise
 */
GamePlayerType GameManager_GetCurrentPlayerType(GameManager *manager);

/**
 * Calculate an AI move using the minimax algorithm (with pruning),
 * according to the given GameManager's difficulty.
 * @param   manager     the instance to work on
 * @return  an AI DO_MOVE command
 */
GameCommand GameManager_GetAIMove(GameManager *manager);

/**
 * Update a GameManger instance according to a given command.
 * @param   manager     the instance to work on
 */
void GameManager_ProcessCommand(GameManager *manager, GameCommand command);

/**
 * Send a formatted string of a given GameManager's game settings to a given stream.
 * Does nothing if either manager == NULL or stream == NULL.
 * @param   manager     the instance to fetch the string from
 * @param   stream      the stream to send the string to
 */
void GameManager_SettingsToStream(const GameManager *manager, FILE *stream);

/**
 * Send a formatted string of a given GameManager's game board to a given stream.
 * Does nothing if either manager == NULL or stream == NULL.
 * @param   manager     the instance to fetch the string from
 * @param   stream      the stream to send the string to
 */
void GameManager_BoardToStream(const GameManager *manager, FILE *stream);

char* chessPieceLocationToStr(ChessGame *game, int x, int y);

static const struct ChessColorToString {
    ChessColor color;
    const char *string;
} ChessColorToString[] = {
    { CHESS_PLAYER_COLOR_BLACK, "black" },
    { CHESS_PLAYER_COLOR_WHITE, "white" },
    { CHESS_PLAYER_COLOR_NONE, "none" },
};


#endif
