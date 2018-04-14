#include <stdbool.h>
#include "UIManager.h"
#include "GameManager.h"


bool shouldQuit(GameManager *gameManager, UIManager *uiManager, GameCommand command) {
    if (UIManager_GetUIType(uiManager) == UI_TYPE_CLI) {
        return gameManager->status == GAME_STATUS_CHECKMATE ||
               gameManager->status == GAME_STATUS_DRAW;
    }
    return command.type == GAME_COMMAND_QUIT;
}

GameCommand getNextCommand(GameManager *gameManager, UIManager *uiManager) {
    if (gameManager->phase == GAME_PHASE_SETTINGS) {
        return UIManager_ProcessInput(uiManager);
    } else {
        switch (GameManager_GetCurrentPlayerType(gameManager)) {
            case GAME_PLAYER_TYPE_HUMAN:
                return UIManager_ProcessInput(uiManager);
            case GAME_PLAYER_TYPE_AI:
                return GameManager_GetAIMove(gameManager);
        }
    }    
}

int main(int argc, const char *argv[]) {
    bool toQuit = false;
    GameManager *gameManager = GameManager_Create();
    if (!gameManager) toQuit = true;
    UIManager *uiManager = UIManager_Create(argc, argv);
    if (!uiManager) toQuit = true;
    GameCommand command = { .type = GAME_COMMAND_INVALID };
    while (true) {
        UIManager_Render(uiManager, gameManager, command);
        toQuit = shouldQuit(gameManager, uiManager, command);
        if (toQuit) break;
        command = getNextCommand(gameManager, uiManager);
        GameManager_ProcessCommand(gameManager, command);
    }
    UIManager_Destroy(uiManager);
    GameManager_Destroy(gameManager);
    return 0;
}
