#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CLIEngine.h"

#define MSG_APP_INIT            " Chess\n-------\n"
#define MSG_SETTINGS_STATE      "Specify game settings or type 'start' to begin a game with the current settings:\n"
#define MSG_GAME_MODE           "Game mode is set to %d%s\n"
#define MSG_DIFFICULTY          "Difficulty level is set to %s\n"
#define MSG_USER_COLOR          "User color is set to %s\n"
#define MSG_DEFAULT_SETTINGS    "All settings reset to default\n"
#define MSG_QUIT                "Exiting...\n"
#define MSG_START               "Starting game...\n"
#define MSG_MAKE_MOVE           "Enter your move (%s player):\n"
#define MSG_CHECKMATE           "Checkmate! %s player wins the game\n"
#define MSG_CHECK               "Check: %s king is threatened\n"
#define MSG_DRAW                "The game ends in a draw\n"
#define MSG_MOVE_DEFAULT        "<%d,%c>\n"
#define MSG_MOVE_THREATENED     "<%d,%c>*\n"
#define MSG_MOVE_CAPTURES       "<%d,%c>^\n"
#define MSG_MOVE_BOTH           "<%d,%c>*^\n"
#define MSG_GAME_SAVED          "Game saved to: %s\n"
#define MSG_UNDO_MOVE           "Undo move for %s player: <%d,%c> -> <%d,%c>\n"
#define MSG_RESTART             "Restarting...\n"
#define MSG_AI_MOVE             "Computer: move %s at <%d,%c> to <%d,%c>\n"

#define INPUT_DELIMITERS        " \t\r\n"
#define MAX_INT_VALUE           50


struct CLIEngine {
    char input[GAME_COMMAND_MAX_LINE_LENGTH];
};

CLIEngine* CLIEngine_Create() {
    CLIEngine *this = malloc(sizeof(CLIEngine));
    if (!this) return NULL;
    printf(MSG_APP_INIT);
    printf(MSG_SETTINGS_STATE);
    return this;
}

void CLIEngine_Destroy(CLIEngine *this) {
    if (!this) return;
    free(this);
}

GameCommandType strToCommandType(const char *str) {
    if (!strcmp(str, "game_mode"))          return GAME_COMMAND_GAME_MODE;
    if (!strcmp(str, "difficulty"))         return GAME_COMMAND_DIFFICULTY;
    if (!strcmp(str, "user_color"))         return GAME_COMMAND_USER_COLOR;
    if (!strcmp(str, "load"))               return GAME_COMMAND_LOAD_GAME;
    if (!strcmp(str, "default"))            return GAME_COMMAND_DEFAULT_SETTINGS;
    if (!strcmp(str, "print_settings"))     return GAME_COMMAND_PRINT_SETTINGS;
    if (!strcmp(str, "start"))              return GAME_COMMAND_START;
    if (!strcmp(str, "move"))               return GAME_COMMAND_MOVE;
    if (!strcmp(str, "get_moves"))          return GAME_COMMAND_GET_MOVES;
    if (!strcmp(str, "save"))               return GAME_COMMAND_SAVE;
    if (!strcmp(str, "undo"))               return GAME_COMMAND_UNDO;
    if (!strcmp(str, "reset"))              return GAME_COMMAND_RESET;
    if (!strcmp(str, "quit"))               return GAME_COMMAND_QUIT;
    return GAME_COMMAND_INVALID;
}

typedef enum GameCommandArgsType {
    COMMAND_ARGS_INTS,
    COMMAND_ARGS_STRING,
    COMMAND_ARGS_MOVES,
    COMMAND_ARGS_NONE,
} GameCommandArgsType;

GameCommandArgsType getCommandArgsType(const GameCommandType commandType) {
    switch (commandType) {
        // COMMAND_ARGS_INTS
        case GAME_COMMAND_GAME_MODE:
        case GAME_COMMAND_DIFFICULTY:
        case GAME_COMMAND_USER_COLOR:
            return COMMAND_ARGS_INTS;
        // COMMAND_ARGS_STRING
        case GAME_COMMAND_SAVE:
        case GAME_COMMAND_LOAD_GAME:
            return COMMAND_ARGS_STRING;
        // COMMAND_ARGS_MOVES
        case GAME_COMMAND_MOVE:
        case GAME_COMMAND_GET_MOVES:
            return COMMAND_ARGS_MOVES;
        // COMMAND_ARGS_NONE
        case GAME_COMMAND_DEFAULT_SETTINGS:
        case GAME_COMMAND_PRINT_SETTINGS:
        case GAME_COMMAND_START:
        case GAME_COMMAND_UNDO:
        case GAME_COMMAND_RESET:
        case GAME_COMMAND_QUIT:
        case GAME_COMMAND_INVALID:
        default:
            return COMMAND_ARGS_NONE;
    }
}

int isInt(const char* str) {
	int i = str[0] == '-' ? 1 : 0;  // case the number is negative
    if ((i && strlen(str) == 1) || str[0] == '0') return false; //case str == '-' or number start with '0'
	while (str[i] != '\0' && str[i] != '\n') {
		if (str[i] < '0' || str[i] > '9') return false;
		i++;
	}
	return true;
}

GameCommand CLIEngine_ProcessInput(CLIEngine *this) {
    GameCommand command = { .type = GAME_COMMAND_INVALID, .args = {-1} };
    if (!this) return command;
    char* input;
    while ((input = fgets(this->input, GAME_COMMAND_MAX_LINE_LENGTH, stdin))){
        if (input[0] == '\n') continue; // handle newline (only) input
        else break;
    }
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
                    command.args[argIndex++] = atoi(token);
                } else if (strlen(token) == 1) {  // should be 'A'-'H'
                    command.args[argIndex++] = token[0];
                } else {
                    command.args[argIndex++] = MAX_INT_VALUE;
                }
            }
            break;
        case COMMAND_ARGS_STRING:
            token = strtok(NULL, INPUT_DELIMITERS);  // should be path string
            if (!token) {
                command.type = GAME_COMMAND_INVALID;
                break;
            }
            strcpy(command.path, token);
            break;  // TODO: consider check for more args and alert somehow
        case COMMAND_ARGS_MOVES:
            token = strtok(NULL, INPUT_DELIMITERS);
            if (!token || strlen(token) != 5 || token[0] != '<' ||
                token[2] != ',' || token[4] != '>' ){ //first position of move command
                command.type = GAME_COMMAND_INVALID; // move command is invalid
                while(token) token = strtok(NULL, INPUT_DELIMITERS);
                break;
            }
            command.args[0] = atoi(&token[1]);
            command.args[1] = token[3];
            token = strtok(NULL, INPUT_DELIMITERS);
            if (!token || strcmp("to", token)) {
                while(token) token = strtok(NULL, INPUT_DELIMITERS);
                if (command.type == GAME_COMMAND_MOVE) command.type = GAME_COMMAND_INVALID;// invalid move syntax
                break;
            }
            token = strtok(NULL, INPUT_DELIMITERS);
            if (!token || strlen(token) != 5 || token[0] != '<' ||
                token[2] != ',' || token[4] != '>' ){
                    command.type = GAME_COMMAND_INVALID;
                    while(token) token = strtok(NULL, INPUT_DELIMITERS);
                    break;
                }
            command.args[2] = atoi(&token[1]);
            command.args[3] = token[3];
            while(token) token = strtok(NULL, INPUT_DELIMITERS);
            break;
        case COMMAND_ARGS_NONE:
            break;
    }
    return command;
}

static const struct GameErrorToString {
    GameError error;
    const char *string;
} GameErrorToString[] = {
    { GAME_ERROR_NONE, "" },
    { GAME_ERROR_INVALID_COMMAND, "ERROR: invalid command\n" },
    { GAME_ERROR_INVALID_GAME_MODE, "Wrong game mode\n" },
    { GAME_ERROR_INVALID_DIFF_LEVEL, "Wrong difficulty level. The value shold be between 1 to 5\n" },
    { GAME_ERROR_INVALID_USER_COLOR, "Wrong user color. The value should be 0 or 1\n" },
    { GAME_ERROR_INVALID_FILE, "ERROR: File doesn’t exist or cannot be opened\n" },
    { GAME_ERROR_INVALID_POSITION, "Invalid position on the board\n" },
    { GAME_ERROR_EMPTY_POSITION, "The specified position does not contain your piece\n" },
    { GAME_ERROR_INVALID_MOVE, "Illegal move\n" },
    { GAME_ERROR_INVALID_MOVE_KING_IS_T, "Illegal move: king is still threatened\n" },
    { GAME_ERROR_INVALID_MOVE_KING_WILL_T, "Ilegal move: king will be threatened\n" },
    { GAME_ERROR_FILE_ALLOC, "File cannot be created or modified\n" },
    { GAME_ERROR_EMPTY_HISTORY, "Empty history, no move to undo\n" },
};

void handleError(const GameManager *manager) {
    printf("%s", GameErrorToString[manager->error].string);
    if (manager->error >= GAME_ERROR_INVALID_POSITION || (manager->error == GAME_ERROR_INVALID_COMMAND && manager->phase == GAME_PHASE_RUNNING)) {
        printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");
    }
}

void CLIEngine_Render(CLIEngine *this, const GameManager *manager, GameCommand command) {
    (void)this;
    if (!manager) return;
    if (manager->error != GAME_ERROR_NONE) {
        handleError(manager);
        return;
    }
    ChessPos *pos;
    ChessMove *move;
    switch(command.type) {
        case GAME_COMMAND_GAME_MODE:
            printf(MSG_GAME_MODE, manager->game->mode, "-player");
            break;
        case GAME_COMMAND_DIFFICULTY:
            switch(manager->game->difficulty){
                case CHESS_DIFFICULTY_AMATEUR:
                    printf(MSG_DIFFICULTY, "amateur");
                    break;
                case CHESS_DIFFICULTY_EASY:
                    printf(MSG_DIFFICULTY, "easy");
                    break;
                case CHESS_DIFFICULTY_MODERATE:
                    printf(MSG_DIFFICULTY, "moderate");
                    break;
                case CHESS_DIFFICULTY_HARD:
                    printf(MSG_DIFFICULTY, "hard");
                    break;  
                case CHESS_DIFFICULTY_EXPERT:
                    printf(MSG_DIFFICULTY, "expert");
                    break;  
            }
            break;
        case GAME_COMMAND_USER_COLOR:
            switch(manager->game->userColor){
                case CHESS_PLAYER_COLOR_WHITE:
                    printf(MSG_USER_COLOR, "white");
                    break;
                case CHESS_PLAYER_COLOR_BLACK:
                    printf(MSG_USER_COLOR, "black");
                    break;
                default:
                    break;
            }
            break;
        case GAME_COMMAND_LOAD_GAME:
            break;
        case GAME_COMMAND_DEFAULT_SETTINGS:
            printf(MSG_DEFAULT_SETTINGS);
            break;
        case GAME_COMMAND_PRINT_SETTINGS:
            GameManager_SettingsToStream(manager, stdout);
            break;
        case GAME_COMMAND_START:
            printf(MSG_START);
            if (manager->game->turn == manager->game->userColor) {
                GameManager_BoardToStream(manager, stdout);
                printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");
            }
            break;
        case GAME_COMMAND_MOVE:
            if (manager->game->mode == CHESS_MODE_2_PLAYER) {
                GameManager_BoardToStream(manager, stdout);
                printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");
            } else {
                if (manager->game->turn == manager->game->userColor) {
                    printf(MSG_AI_MOVE, chessPieceLocationToStr(manager->game, command.args[3]-'A', command.args[2]-1), command.args[0], command.args[1], command.args[2], command.args[3]);
                    GameManager_BoardToStream(manager, stdout);
                    printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");
                }
            }
            break;
        case GAME_COMMAND_GET_MOVES:
            while (!ArrayStack_IsEmpty(manager->moves)) {
                pos = ArrayStack_Pop(manager->moves);
                switch (pos->type) {
                    case CHESS_POS_STANDARD:
                        printf(MSG_MOVE_DEFAULT, pos->y + 1, pos->x + 'A');
                        break;
                    case CHESS_POS_THREATENED:
                        printf(MSG_MOVE_THREATENED, pos->y + 1, pos->x + 'A');
                        break;
                    case CHESS_POS_CAPTURE:
                        printf(MSG_MOVE_CAPTURES, pos->y + 1, pos->x + 'A');
                        break;
                    case CHESS_POS_BOTH:
                        printf(MSG_MOVE_BOTH, pos->y + 1, pos->x + 'A');
                        break;
                    default:
                        break;
                }
            }
            printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");
            break;
        case GAME_COMMAND_SAVE:
            printf(MSG_GAME_SAVED, command.path);
            break;
        case GAME_COMMAND_UNDO:
            while (!ArrayStack_IsEmpty(manager->moves)) {
                move = ArrayStack_Pop(manager->moves);
                printf(MSG_UNDO_MOVE, ChessColorToString[move->player].string, move->to.y + 1 ,move->to.x + 'A', move->from.y + 1 ,move->from.x + 'A');
            }
            GameManager_BoardToStream(manager, stdout);
            printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");
            break;
        case GAME_COMMAND_RESET:
            printf(MSG_RESTART);
            printf(MSG_SETTINGS_STATE);
            break;
        case GAME_COMMAND_QUIT:
            printf(MSG_QUIT);
            return;
        default:
            break;
    }
}
