#include <stdbool.h>
#include "UIManager.h"
#include "GameManager.h"


bool toQuit(GameManager *gameManager, UIManager *uiManager, GameCommand command) {
    if (!gameManager || !uiManager) return true;
    if (UIManager_GetUIType(uiManager) == UI_TYPE_CLI &&
        (gameManager->status == GAME_STATUS_CHECKMATE ||
         gameManager->status == GAME_STATUS_DRAW)) return true;
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
    return (GameCommand){ .type = GAME_COMMAND_INVALID };
}

int main(int argc, const char *argv[]) {
    GameManager *gameManager = GameManager_Create();
    UIManager *uiManager = UIManager_Create(argc, argv);
    GameCommand command = { .type = GAME_COMMAND_INVALID };
    while (true) {
        UIManager_Render(uiManager, gameManager, command);
        if (toQuit(gameManager, uiManager, command)) break;
        command = getNextCommand(gameManager, uiManager);
        GameManager_ProcessCommand(gameManager, command);
    }
    UIManager_Destroy(uiManager);
    GameManager_Destroy(gameManager);
    return 0;
}
