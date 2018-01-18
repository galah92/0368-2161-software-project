#include <stdio.h>
#include <string.h>
#include "ChessGame.h"
#include "UserCommand.h"


typedef enum {
    SETTINGS_STATE,
    GAME_STATE,
    QUIT,
    RESTARTED,
} AppState;


void HandleSettingsState(ChessGame *game) {
    printf("Specify game settings or type 'start' to begin a game with the current settings:\n");
    UserCommand command;
    while (command.type != USER_COMMAND_START) {
        // TODO: update 'command' from user input
        switch (command.type) {
            case USER_COMMAND_INVALID:
                printf("ERROR: invalid command\n");
                break;
            case USER_COMMAND_GAME_MODE:
                ChessGame_SetGameMode(game, command.params[0]);
                char *mode = command.params[0] == CHESS_GAME_MODE_1_PLAYER ? "1-player" : "2-player";
                printf("Game mode is set to %s\n", mode);
                break;
            case USER_COMMAND_DIFFICULTY:
                ChessGame_SetDifficulty(game, command.params[0]);
                break;
            case USER_COMMAND_USER_COLOR:
                ChessGame_SetUserColor(game, command.params[0]);
                break;
            case USER_COMMAND_LOAD_GAME:
                // TODO: implement
                break;
            case USER_COMMAND_DEFAULT_SETTINGS:
                ChessGame_SetDefaultSettings(game);
                break;
            case USER_COMMAND_PRINT_SETTINGS:
                ChessGame_PrintSettings(game);
                break;
            case USER_COMMAND_QUIT:
                break;
            case USER_COMMAND_START:
                printf("Starting game…\n");
                return;
            case USER_COMMAND_MOVE: break;
            case USER_COMMAND_GET_MOVES: break;
            case USER_COMMAND_SAVE: break;
            case USER_COMMAND_UNDO: break;
            case USER_COMMAND_RESET: break;
        }
    }
}

int main(int argc, char *argv[]) {
    AppState state = SETTINGS_STATE;
    // ChessGame *game;
    if (argc > 1 && strcmp(argv[1], "-g") == 0) {
        printf("GUI mode - WIP\n");
    } else {
        printf(" Chess\n-------\n");
        while (state != QUIT) {
            if (state == RESTARTED) {
                // game = ChessGame_Create();
                state = GAME_STATE;
            } else {

            }
        }
    }
    return 0;
}
