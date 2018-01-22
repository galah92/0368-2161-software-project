#ifndef GAME_COMMAND_H_
#define GAME_COMMAND_H_

#define GAME_COMMAND_MAX_LINE_LENGTH    1024
#define GAME_COMMAND_ARGS_CAPACITY    8


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


typedef struct GameCommand_t {
	GameCommandType type;
	int args[GAME_COMMAND_ARGS_CAPACITY];
    char path[GAME_COMMAND_MAX_LINE_LENGTH];
} GameCommand;


#endif
