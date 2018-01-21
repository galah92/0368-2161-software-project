#include <stdio.h>
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



CLIEngine* CLIEngine_Create() {
    CLIEngine *engine = malloc(sizeof(CLIEngine));
    if (!engine) return NULL;
    return engine;
}

void CLIEngine_Destroy(CLIEngine *engine) {
    if (!engine) return;
    free(engine);
}

UserCommand* CLIEngine_ProcessInput(CLIEngine *engine) {
    if (!engine) return NULL;
    // TODO: Parse CLI strings to UserCommand;
    UserCommand *command = malloc(sizeof(UserCommand));
    return command;
}

void CLIEngine_Render(CLIEngine *engine, const GameState *state) {
    // TODO: All CLI printing should go here basically
    if (!state) return;
    printf("CLIEngine_Render:\t%d\n", engine->_x);
    if (state->state == ERROR) {
        if (state->error.errorType == ERROR_CUSTOME) {
            printf("%s", state->error.description);
        } else {
            
        }
    }
}
