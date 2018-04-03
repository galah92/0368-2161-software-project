#include "GameManager.h"


void processSettingsCommand(GameManager *manager, GameCommand command) {
    (void)manager;
    switch (command.type) {
        case GAME_COMMAND_GAME_MODE:
            if      (command.args[0] == 0)       manager->game->mode = CHESS_MODE_1_PLAYER; // one player mode
            else if (command.args[0] == 1)       manager->game->mode = CHESS_MODE_2_PLAYER; // two player mode
            else                                 manager->error = GAME_ERROR_INVALID_GAME_MODE; // wrong args/no args supplied
            break;
        case GAME_COMMAND_DIFFICULTY:
            if (command.args[0] <= 4 && command.args[0] >= 0){ // currect value
                switch(command.args[0]){
                    case 0:         manager->game->difficulty = CHESS_DIFFICULTY_AMATEUR;        break;
                    case 1:         manager->game->difficulty = CHESS_DIFFICULTY_EASY;           break;
                    case 2:         manager->game->difficulty = CHESS_DIFFICULTY_MODERATE;       break;
                    case 3:         manager->game->difficulty = CHESS_DIFFICULTY_HARD;           break;
                    case 4:         manager->game->difficulty = CHESS_DIFFICULTY_EXPERT;         break;
                    default: break;
                }
            } else manager->error = GAME_ERROR_INVALID_DIFF_LEVEL;
            break;
        case GAME_COMMAND_USER_COLOR:
            if      (command.args[0] == -1)         manager->game->userColor = CHESS_PLAYER_COLOR_WHITE; // user color 0 = white
            else if (command.args[0] == 0)          manager->game->userColor = CHESS_PLAYER_COLOR_BLACK; // user color 1 = black
            else                                    manager->error = GAME_ERROR_INVALID_USER_COLOR;
            break;
        case GAME_COMMAND_LOAD_GAME:
            break;
        case GAME_COMMAND_DEFAULT_SETTINGS:
            break;
        case GAME_COMMAND_PRINT_SETTINGS:
            break;
        case GAME_COMMAND_START:
            break;
        case GAME_COMMAND_QUIT:
            break;
        case GAME_COMMAND_INVALID:
        default:
            break;
    }
}

void processRunningCommand(GameManager *manager, GameCommand command) {
    (void)manager;
    switch (command.type) {
        case GAME_COMMAND_MOVE:
            break;
        case GAME_COMMAND_GET_MOVES:
            break;
        case GAME_COMMAND_SAVE:
            break;
        case GAME_COMMAND_UNDO:
            break;
        case GAME_COMMAND_RESET:
            break;
        case GAME_COMMAND_QUIT:
            break;
        case GAME_COMMAND_INVALID:
        default:
            break;
    }
}

void GameManager_ProcessCommand(GameManager *manager, GameCommand command) {
    if (manager->phase == GAME_PHASE_SETTINGS) {
        processSettingsCommand(manager, command);
    } else if (manager->phase == GAME_PHASE_RUNNING) {
        processRunningCommand(manager, command);
    }
}
