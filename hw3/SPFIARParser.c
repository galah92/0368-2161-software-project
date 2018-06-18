#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "SPFIARParser.h"


bool spParserIsInt(const char* str) {
	int i = str[0] == '-' ? 1 : 0;  // case the number is negative
	while (str[i] != '\0' && str[i] != '\n') {
		if (str[i] < '0' || str[i] > '9') return false;
		i++;
	}
	return true;
}

SPCommand spParserPraseLine(const char* str) {
	SPCommand command = { .cmd = SP_INVALID_LINE, .validArg = false };
	char line[SP_MAX_LINE_LENGTH];
	const char delim[] = " \r\t\n";
	strcpy(line, str);
	char *cmd = strtok(line, delim);  // first part of the command
	if (strcmp(cmd, "undo_move") == 0) {
		command.cmd = SP_UNDO_MOVE;
	} else if (strcmp(cmd, "suggest_move") == 0) {
		command.cmd = SP_SUGGEST_MOVE;
	} else if (strcmp(cmd, "quit") == 0) {
		command.cmd = SP_QUIT;
	} else if (strcmp(cmd, "restart_game") == 0) {
		command.cmd = SP_RESTART;
	} else if (strcmp(cmd, "add_disc") == 0) {
		command.cmd = SP_ADD_DISC;
	}
	cmd = strtok(NULL, delim);  // second part of the command
	if (cmd && spParserIsInt(cmd)) {
		if (command.cmd == SP_ADD_DISC) {
			command.arg = atoi(cmd);
			command.validArg = true;
		} else {  // shouldn't recieved args
			command.cmd = SP_INVALID_LINE;
		}
	}
	return command;
}
