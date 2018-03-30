#include "GameManager.h"


void processSettingsCommand(GameManager *manager, GameCommand command) {
    (void)manager;
    switch (command.type) {
        case GAME_COMMAND_GAME_MODE:
            
            break;
        case GAME_COMMAND_DIFFICULTY:
            break;
        case GAME_COMMAND_USER_COLOR:
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
