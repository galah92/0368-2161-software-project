#ifndef USER_COMMAND_H_
#define USER_COMMAND_H_

#define USER_COMMAND_MAX_LINE_LENGTH 1024
#define USER_COMMAND_MAX_PARAMS_NUM 4


typedef enum UserCommandType_t {
    // settings-only commands
	USER_COMMAND_GAME_MODE,
	USER_COMMAND_DIFFICULTY,
	USER_COMMAND_USER_COLOR,
    USER_COMMAND_LOAD_GAME,
    USER_COMMAND_DEFAULT_SETTINGS,
    USER_COMMAND_PRINT_SETTINGS,
    USER_COMMAND_START,
    // game-only commands
    USER_COMMAND_MOVE,
    USER_COMMAND_GET_MOVES,
    USER_COMMAND_SAVE,
    USER_COMMAND_UNDO,
    USER_COMMAND_RESET,
    // shared commands
    USER_COMMAND_QUIT,
    USER_COMMAND_INVALID,
} UserCommandType;


typedef struct UserCommand_t {
	UserCommandType type;
	int params[USER_COMMAND_MAX_PARAMS_NUM];
    char path[USER_COMMAND_MAX_LINE_LENGTH];
} UserCommand;


UserCommand UserCommand_ParseSettingsCommand(char* str);

UserCommand UserCommand_ParseGameCommand(char* str);


#endif
