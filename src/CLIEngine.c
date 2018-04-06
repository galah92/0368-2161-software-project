#include <stdio.h>
#include <string.h>
#include "CLIEngine.h"

#define MSG_APP_INIT                    " Chess\n-------\n"
#define MSG_SETTINGS_STATE              "Specify game settings or type 'start' to begin a game with the current settings:\n"
#define MSG_GAME_MODE                   "Game mode is set to %d%s\n"
#define MSG_DIFFICULTY                  "Difficulty level is set to %s\n"
#define MSG_USER_COLOR                  "User color is set to %s\n"
#define MSG_DEFAULT_SETTINGS            "All settings reset to default\n"
#define MSG_QUIT                        "Exiting...\n"
#define MSG_START                       "Starting game...\n"
#define MSG_MAKE_MOVE                   "Enter your move (%s player):\n"
#define MSG_CHECKMATE                   "Checkmate! %s player wins the game\n"
#define MSG_CHECK                       "Check: %s king is threatened\n"
#define MSG_DRAW                        "The game ends in a draw\n"
#define MSG_MOVE_DEFAULT                "<%d,%c>\n"
#define MSG_MOVE_THREATENED             "<%d,%c>*\n"
#define MSG_MOVE_CAPTURES               "<%d,%c>^\n"
#define MSG_MOVE_BOTH                   "<%d,%c>*^\n"
#define MSG_GAME_SAVED                  "Game saved to: %s\n"
#define MSG_UNDO_MOVE                   "Undo move for %s player: <%d,%c> -> <%d,%c>\n"
#define MSG_RESTART                     "Restarting...\n"
#define MSG_AI_MOVE                     "Computer: move %s at <%d,%c> to <%d,%c>\n"

#define MSG_ERR_INVALID_COMMAND             "ERROR: invalid command\n"
#define MSG_ERR_WRONG_GAME_MODE             "Wrong game mode\n"
#define MSG_ERR_WRONG_DIFF_LEVEL            "Wrong difficulty level. The value should be between 1 to 5\n"
#define MSG_ERR_WRONG_USER_COLOR            "Wrong user color. The value should be 0 or 1\n"
#define MSG_ERR_FILE_NOT_EXIST              "ERROR: File doesn’t exist or cannot be opened\n"
#define MSG_ERR_INVALID_POSITION            "Invalid position on the board\n"
#define MSG_ERR_POSITION_EMPTY              "The specified position does not contain your piece\n"
#define MSG_ERR_ILLEGAL_MOVE                "Illegal move\n"
#define MSG_ERR_ILLEGAL_MOVE_KING_IS_T      "Illegal move: king is still threatened\n"
#define MSG_ERR_ILLEGAL_MOVE_KING_WILL_T    "Ilegal move: king will be threatened\n"
#define MSG_ERR_FILE_CANNOT_BE_CREATED      "File cannot be created or modified\n"
#define MSG_ERR_EMPTY_HISTORY               "Empty history, no move to undo\n"

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
#define MAX_INT_VALUE                   50

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
    if (!strcmp(str, IN_STRING_GAME_MODE))  return GAME_COMMAND_GAME_MODE;
    if (!strcmp(str, IN_STRING_DIFFICULTY)) return GAME_COMMAND_DIFFICULTY;
    if (!strcmp(str, IN_STRING_USER_COLOR)) return GAME_COMMAND_USER_COLOR;
    if (!strcmp(str, IN_STRING_LOAD_GAME))  return GAME_COMMAND_LOAD_GAME;
    if (!strcmp(str, IN_STRING_DEFAULT))    return GAME_COMMAND_DEFAULT_SETTINGS;
    if (!strcmp(str, IN_STRING_PRINT))      return GAME_COMMAND_PRINT_SETTINGS;
    if (!strcmp(str, IN_STRING_START))      return GAME_COMMAND_START;
    if (!strcmp(str, IN_STRING_MOVE))       return GAME_COMMAND_MOVE;
    if (!strcmp(str, IN_STRING_GET_MOVES))  return GAME_COMMAND_GET_MOVES;
    if (!strcmp(str, IN_STRING_SAVE))       return GAME_COMMAND_SAVE;
    if (!strcmp(str, IN_STRING_UNDO))       return GAME_COMMAND_UNDO;
    if (!strcmp(str, IN_STRING_RESET))      return GAME_COMMAND_RESET;
    if (!strcmp(str, IN_STRING_QUIT))       return GAME_COMMAND_QUIT;
    return GAME_COMMAND_INVALID;
}

typedef enum GameCommandArgsType_t {
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
	while (str[i] != '\0' && str[i] != '\n') {
		if (str[i] < '0' || str[i] > '9') return 0;
		i++;
	}
	return 1;
}

GameCommand CLIEngine_ProcessInput(CLIEngine *this) {
    GameCommand command = { .type = GAME_COMMAND_INVALID, .args = {-1} };
    if (!this) return command;
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

void handleError(const GameManager *manager) {
    switch (manager->error) {
        case GAME_ERROR_INVALID_COMMAND:
            printf(MSG_ERR_INVALID_COMMAND);
            return;
        case GAME_ERROR_INVALID_GAME_MODE:
            printf(MSG_ERR_WRONG_GAME_MODE);
            return;
        case GAME_ERROR_INVALID_DIFF_LEVEL:
            printf(MSG_ERR_WRONG_DIFF_LEVEL);
            return;
        case GAME_ERROR_INVALID_USER_COLOR:
            printf(MSG_ERR_WRONG_USER_COLOR);
            return;
        case GAME_ERROR_INVALID_FILE:
            printf(MSG_ERR_FILE_NOT_EXIST);
            return;
        case GAME_ERROR_INVALID_POSITION:
            printf(MSG_ERR_INVALID_POSITION);
            printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");
            return;
        case GAME_ERROR_EMPTY_POSITION:
            printf(MSG_ERR_POSITION_EMPTY);
            printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");
            return;
        case GAME_ERROR_INVALID_MOVE:
            printf(MSG_ERR_ILLEGAL_MOVE);
            printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");
            return;
        case GAME_ERROR_INVALID_MOVE_KING_IS_T:
            printf(MSG_ERR_ILLEGAL_MOVE_KING_IS_T);
            printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");
            return;
        case GAME_ERROR_INVALID_MOVE_KING_WILL_T:
            printf(MSG_ERR_ILLEGAL_MOVE_KING_WILL_T);
            printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");
            return;
        case GAME_ERROR_FILE_ALLOC:
            printf(MSG_ERR_FILE_CANNOT_BE_CREATED);
            return;
        case GAME_ERROR_EMPTY_HISTORY:
            printf(MSG_ERR_EMPTY_HISTORY);
            printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");            
            return;
        case GAME_ERROR_NONE:
        default:
            break;
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
            ChessGame_SettingsToStream(manager->game, stdout);
            break;
        case GAME_COMMAND_START:
            printf(MSG_START);
            if (manager->game->turn == manager->game->userColor){
                ChessGame_BoardToStream(manager->game, stdout);
                printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");
            }
            break;
        case GAME_COMMAND_MOVE:
            if (manager->game->mode == CHESS_MODE_2_PLAYER){
                ChessGame_BoardToStream(manager->game, stdout);
                printf(MSG_MAKE_MOVE, manager->game->turn == CHESS_PLAYER_COLOR_WHITE ? "white" : "black");
            } else {
                if (manager->game->turn == manager->game->userColor){
                    printf(MSG_AI_MOVE, chessPieceLocationToStr(manager->game,command.args[3]-'A' ,command.args[2]-1),command.args[0],command.args[1],command.args[2],command.args[3]);
                    ChessGame_BoardToStream(manager->game, stdout);
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
                printf(MSG_UNDO_MOVE, chessColorToColorStr(move->player), move->to.y + 1 ,move->to.x + 'A', move->from.y + 1 ,move->from.x + 'A');
            }
            ChessGame_BoardToStream(manager->game, stdout);
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
