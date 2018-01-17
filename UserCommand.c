#include "UserCommand.h"


UserCommand UserCommand_Parse(const char* str) {
    UserCommand command = { .type = USER_COMMAND_INVALID, .params = { 0, 0, 0, 0 } };
    return command;
}
