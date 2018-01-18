#include <string.h>
#include "UserCommand.h"


UserCommand UserCommand_Parse(char *str) {
    UserCommand command = { .type = USER_COMMAND_INVALID, .params = { 0, 0, 0, 0 } };
    if (strcmp(str, "start")) {
        command.type = USER_COMMAND_START;
    }
    return command;
}
