#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

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
    ArrayStack *moves;
    GameStatus status;
} GameManager;

GameManager* GameManager_Create();

void GameManager_Destroy(GameManager *manager);

GamePlayerType GameManager_GetCurrentPlayerType(GameManager *manager);

GameCommand GameManager_GetAIMove(GameManager *manager);

void GameManager_ProcessCommand(GameManager *manager, GameCommand command);

/**
 * Send a formatted string of a given ChessGame's settings to a given stream.
 * @param   game        the instance to fetch the string from
 * @param   stream      the stream to send the string to
 * return   CHESS_INVALID_ARGUMENT if game == NULL || stream == NULL
 *          CHESS_SUCCESS otherwise
 */
void GameManager_SettingsToStream(const GameManager *manager, FILE *stream);

/**
 * Send a formatted string of a given ChessGame's board to a given stream.
 * @param   game        the instance to fetch the string from
 * @param   stream      the stream to send the string to
 * return   CHESS_INVALID_ARGUMENT if game == NULL || stream == NULL
 *          CHESS_SUCCESS otherwise
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
