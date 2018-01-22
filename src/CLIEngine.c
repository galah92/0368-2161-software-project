#include <stdio.h>
#include <string.h>
#include "CLIEngine.h"

#define MSG_APP_INIT                    " Chess\n-------\n"
#define MSG_SETTINGS_STATE              "Specify game settings or type 'start' to begin a game with the current settings:\n"
#define MSG_GAME_MODE                   "Game mode is set to %s\n"
#define MSG_DIFFICULTY                  "Difficulty level is set to %s\n"
#define MSG_USER_COLOR                  "User color is set to %s\n"
#define MSG_DEFAULT_SETTINGS            "All settings reset to default\n"
#define MSG_QUIT                        "Exiting...\n"
#define MSG_START                       "Starting game…\n"
#define MSG_MAKE_MOVE                   "Enter your move (%s player):\n"
#define MSG_CHECKMATE                   "Checkmate! %s player wins the game\n"
#define MSG_CHECK                       "Check: %s king is threatened\n"
#define MSG_DRAW                        "The game ends in a draw\n"
#define MSG_MOVE_DEFAULT                "<%c,%c>\n"
#define MSG_MOVE_THREATENED             "<%c,%c>*\n"
#define MSG_MOVE_CAPTURES               "<%c,%c>^\n"
#define MSG_MOVE_BOTH                   "<%c,%c>*^\n"
#define MSG_GAME_SAVED                  "Game saved to: %s\n"
#define MSG_UNDO_MOVE                   "Undo move for %s player: <%c,%c> -> <%c,%c>\n"
#define MSG_RESTART                     "Restarting...\n"
#define MSG_AI_MOVE                     "Computer: move %s at <%c,%c> to <%c,%c>\n"

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

#define IN_STRING_GAME_MODE             "game_mode"
#define IN_STRING_DIFFICULTY            "difficulty"
#define IN_STRING_USER_COLOR            "user_color"
#define IN_STRING_LOAD_GAME             "load"
#define IN_STRING_DEFAULT               "default"
#define IN_STRING_PRINT                 "print_settings"
#define IN_STRING_START                 "start"
#define IN_STRING_MOVE                  "move"
#define IN_STRING_GET_MOVES             "get_moves"
#define IN_STRING_SAVE                  "save"
#define IN_STRING_UNDO                  "undo"
#define IN_STRING_RESET                 "reset"
#define IN_STRING_QUIT                  "quit"

#define INPUT_DELIMITERS                " \t\r\n"


struct CLIEngine {
    char input[GAME_COMMAND_MAX_LINE_LENGTH];
};


CLIEngine* CLIEngine_Create() {
    CLIEngine *this = malloc(sizeof(CLIEngine));
    if (!this) return NULL;
    printf(MSG_APP_INIT);
    return this;
}

void CLIEngine_Destroy(CLIEngine *this) {
    if (!this) return;
    free(this);
}

GameCommandType strToCommandType(const char *str) {
    if (!strcmp(str, IN_STRING_GAME_MODE)) return GAME_COMMAND_GAME_MODE;
    if (!strcmp(str, IN_STRING_DIFFICULTY)) return GAME_COMMAND_DIFFICULTY;
    if (!strcmp(str, IN_STRING_USER_COLOR)) return GAME_COMMAND_USER_COLOR;
    if (!strcmp(str, IN_STRING_LOAD_GAME)) return GAME_COMMAND_LOAD_GAME;
    if (!strcmp(str, IN_STRING_DEFAULT)) return GAME_COMMAND_DEFAULT_SETTINGS;
    if (!strcmp(str, IN_STRING_PRINT)) return GAME_COMMAND_PRINT_SETTINGS;
    if (!strcmp(str, IN_STRING_START)) return GAME_COMMAND_START;
    if (!strcmp(str, IN_STRING_MOVE)) return GAME_COMMAND_MOVE;
    if (!strcmp(str, IN_STRING_GET_MOVES)) return GAME_COMMAND_GET_MOVES;
    if (!strcmp(str, IN_STRING_SAVE)) return GAME_COMMAND_SAVE;
    if (!strcmp(str, IN_STRING_UNDO)) return GAME_COMMAND_UNDO;
    if (!strcmp(str, IN_STRING_RESET)) return GAME_COMMAND_RESET;
    if (!strcmp(str, IN_STRING_QUIT)) return GAME_COMMAND_QUIT;
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

GameCommand CLIEngine_ProcessInput(CLIEngine *this) {
    GameCommand command = { .type = GAME_COMMAND_INVALID };
    if (!this) return command;
    printf(MSG_SETTINGS_STATE);
    char* input = fgets(this->input, GAME_COMMAND_MAX_LINE_LENGTH, stdin);
    if (!input) return command;
    unsigned int lastCharIndex = strlen(this->input) - 1;
    if (this->input[lastCharIndex] == '\n') {
        this->input[lastCharIndex] = '\0';  // trim possible EOL char
    }
    char *token = strtok(this->input, INPUT_DELIMITERS);  // get first token
    command.type = strToCommandType(token);
    switch (getCommandArgsType(command.type)) {
        case COMMAND_ARGS_INTS:
            for (int i = 0, argIndex = 0; i < GAME_COMMAND_ARGS_CAPACITY; i++) {
                token = strtok(NULL, INPUT_DELIMITERS);
                if (!token) break;  // end of args
                if (isInt(token)) {  // should be 1-8
                    command.args[argIndex++] = atoi(token) - 1;
                } else if (strlen(token) == 1) {  // should be 'A'-'H'
                    command.args[argIndex++] = token[0] - 'A';
                }
            }
            break;
        case COMMAND_ARGS_STRING:
            token = strtok(NULL, INPUT_DELIMITERS);  // should be path string
            strcpy(command.path, token);
            break;  // TODO: consider check for more args and alert somehow
        case COMMAND_ARGS_NONE:
            break;
    }
    return command;
}

void CLIEngine_Render(CLIEngine *this, const GameState *state) {
    // TODO: All CLI printing should go here basically
    if (!state) return;
    printf("CLIEngine_Render:\t%s\n", this->input);
    if (state->state == ERROR) {
        if (state->error.errorType == ERROR_CUSTOME) {
            printf("%s", state->error.description);
        } else {
            
        }
    }
}
