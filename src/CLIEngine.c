#include <stdio.h>
#include <string.h>
#include "CLIEngine.h"

#define ERROR_INVALID_COMMAND           "ERROR: invalid command\n"
#define ERROR_WRONG_GAME_MODE           "Wrong game mode\n"
#define ERROR_WRONG_DIFF_LEVEL          "Wrong difficulty level. The value should be between 1 to 5\n"
#define ERROR_WRONG_USER_COLOR          "Wrong user color. The value should be 0 or 1\n"
#define ERROR_FILE_NOT_EXIST            "ERROR: File doesn’t exist or cannot be opened\n"
#define ERROR_INVALID_POSITION          "Invalid position on the board\n"
#define ERROR_POSITION_EMPTY            "The specified position does not contain your piece\n"
#define ERROR_ILLEGAL_MOVE              "Illegal move\n"
#define ERROR_ILLEGAL_MOVE_KING_IS_T    "Illegal move: king is still threatened\n"
#define ERROR_ILLEGAL_MOVE_KING_WILL_T  "Ilegal move: king will be threatened\n"
#define ERROR_FILE_CANNOT_BE_CREATED    "File cannot be created or modified\n"
#define ERROR_EMPTY_HISTORY             "Empty history, no move to undo\n"

#define STRING_GAME_MODE        "game_mode"
#define STRING_DIFFICULTY       "difficulty"
#define STRING_USER_COLOR       "user_color"
#define STRING_LOAD_GAME        "load"
#define STRING_DEFAULT_SETTINGS "default"
#define STRING_PRINT_SETTINGS   "print_settings"
#define STRING_START            "start"
#define STRING_MOVE             "move"
#define STRING_GET_MOVES        "get_moves"
#define STRING_SAVE             "save"
#define STRING_UNDO             "undo"
#define STRING_RESET            "reset"
#define STRING_QUIT             "quit"


struct CLIEngine_t {
    char input[GAME_COMMAND_MAX_LINE_LENGTH];
};

CLIEngine* CLIEngine_Create() {
    CLIEngine *engine = malloc(sizeof(CLIEngine));
    if (!engine) return NULL;
    return engine;
}

void CLIEngine_Destroy(CLIEngine *engine) {
    if (!engine) return;
    free(engine);
}

GameCommandType strToCommandType(char *str) {
    if (!strcmp(str, STRING_GAME_MODE)) return GAME_COMMAND_GAME_MODE;
    if (!strcmp(str, STRING_DIFFICULTY)) return GAME_COMMAND_DIFFICULTY;
    if (!strcmp(str, STRING_USER_COLOR)) return GAME_COMMAND_USER_COLOR;
    if (!strcmp(str, STRING_LOAD_GAME)) return GAME_COMMAND_LOAD_GAME;
    if (!strcmp(str, STRING_DEFAULT_SETTINGS)) return GAME_COMMAND_DEFAULT_SETTINGS;
    if (!strcmp(str, STRING_PRINT_SETTINGS)) return GAME_COMMAND_PRINT_SETTINGS;
    if (!strcmp(str, STRING_START)) return GAME_COMMAND_START;
    if (!strcmp(str, STRING_MOVE)) return GAME_COMMAND_MOVE;
    if (!strcmp(str, STRING_GET_MOVES)) return GAME_COMMAND_GET_MOVES;
    if (!strcmp(str, STRING_SAVE)) return GAME_COMMAND_SAVE;
    if (!strcmp(str, STRING_UNDO)) return GAME_COMMAND_UNDO;
    if (!strcmp(str, STRING_RESET)) return GAME_COMMAND_RESET;
    if (!strcmp(str, STRING_QUIT)) return GAME_COMMAND_QUIT;
    return GAME_COMMAND_INVALID;
}

typedef enum GameCommandArgsType_t {
    COMMAND_ARGS_INTS,
    COMMAND_ARGS_STRING,
    COMMAND_ARGS_NONE,
} GameCommandArgsType;

int getCommandArgsType(const GameCommandType commandType) {
    switch (commandType) {
        // COMMAND_ARGS_INTS
        case GAME_COMMAND_GAME_MODE: return COMMAND_ARGS_INTS;
        case GAME_COMMAND_DIFFICULTY: return COMMAND_ARGS_INTS;
        case GAME_COMMAND_USER_COLOR: return COMMAND_ARGS_INTS;
        case GAME_COMMAND_MOVE: return COMMAND_ARGS_INTS;
        case GAME_COMMAND_GET_MOVES: return COMMAND_ARGS_INTS;
        // COMMAND_ARGS_STRING
        case GAME_COMMAND_SAVE: return COMMAND_ARGS_STRING;
        case GAME_COMMAND_LOAD_GAME: return COMMAND_ARGS_STRING;
        // COMMAND_ARGS_NONE
        case GAME_COMMAND_DEFAULT_SETTINGS: return COMMAND_ARGS_NONE;
        case GAME_COMMAND_PRINT_SETTINGS: return COMMAND_ARGS_NONE;
        case GAME_COMMAND_START: return COMMAND_ARGS_NONE;
        case GAME_COMMAND_UNDO: return COMMAND_ARGS_NONE;
        case GAME_COMMAND_RESET: return COMMAND_ARGS_NONE;
        case GAME_COMMAND_QUIT: return COMMAND_ARGS_NONE;
        case GAME_COMMAND_INVALID: return COMMAND_ARGS_NONE;
        default: return COMMAND_ARGS_NONE;
    }
}

int isInt(const char* str) {
	int i = str[0] == '-' ? 1 : 0;  // case the number is negative
	while (str[i] != '\0' && str[i] != '\n') {
		if (str[i] < '0' || str[i] > '9') return 0;
		i++;
	}
	return 1;
}

GameCommand CLIEngine_ProcessInput(CLIEngine *engine) {
    GameCommand command = { .type = GAME_COMMAND_INVALID };
    if (!engine) return command;
    // TODO: Parse CLI strings to GameCommand;
    char* input = fgets(engine->input, GAME_COMMAND_MAX_LINE_LENGTH, stdin);
    if (!input) return command;
    unsigned int lastCharIndex = strlen(engine->input) - 1;
    if (engine->input[lastCharIndex] == '\n') {
        engine->input[lastCharIndex] = '\0';  // trim possible EOL char
    }
    char *token = strtok(engine->input, " \t\r\n");  // get first token
    command.type = strToCommandType(token);
    switch (getCommandArgsType(command.type)) {
        case COMMAND_ARGS_INTS:
            for (int i = 0, argIndex = 0; i < GAME_COMMAND_ARGS_CAPACITY; i++) {
                token = strtok(NULL, " \t\r\n");
                if (!token) break;  // end of args
                if (isInt(token)) {  // should be 1-8
                    command.args[argIndex++] = atoi(token) - 1;
                } else if (strlen(token) == 1) {  // should be 'A'-'H'
                    command.args[argIndex++] = token[0] - 'A';
                }
            }
            break;
        case COMMAND_ARGS_STRING:
            token = strtok(NULL, " \t\r\n");  // should be path string
            strcpy(command.path, token);
            // TODO: consider check for more args and alert somehow
            break;
        case COMMAND_ARGS_NONE:
            break;
    }
    return command;
}

void CLIEngine_Render(CLIEngine *engine, const GameState *state) {
    // TODO: All CLI printing should go here basically
    if (!state) return;
    printf("CLIEngine_Render:\t%s\n", engine->input);
    if (state->state == ERROR) {
        if (state->error.errorType == ERROR_CUSTOME) {
            printf("%s", state->error.description);
        } else {
            
        }
    }
}
