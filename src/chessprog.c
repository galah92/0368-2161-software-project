#include <stdio.h>
#include "UIManager.h"
#include "GameManager.h"


int main(int argc, const char *argv[]) {
    UIManager *uiManager = UIManager_Create(argc, argv);
    GameManager *gameManager = GameManager_Create();
    GameCommand command;
    while (gameManager->phase != GAME_PHASE_QUIT) {
        switch (GameManager_GetCurrentPlayerType(gameManager)) {
            case GAME_PLAYER_TYPE_HUMAN:
                command = UIManager_ProcessInput(uiManager);
                break;
            case GAME_PLAYER_TYPE_AI:
                // command = AIEngine_GetMoveCommand(gameManager->game);
                break;
        }
        GameManager_ProcessCommand(gameManager, command);
        UIManager_Render(uiManager, gameManager, command);
    }
    GameManager_Destroy(gameManager);
    UIManager_Destroy(uiManager);
    return 0;
}
