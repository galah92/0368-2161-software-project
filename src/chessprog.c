#include "UIManager.h"


int main(int argc, char *argv[]) {
    UIManager *uiManager = UIManager_Create(argc, argv);
    // GameManager *gameManager = GameManager_Create();
    // UserCommand *command;
    // while (1) {
    //     command = UIManager_ProcessInput(uiManager);
    //     GameManager_ProcessCommand(gameManager, command);
    //     UIManager_Render(uiManager, gameManager->gameState);
    // }
    UIManager_Destroy(uiManager);
    // GameManager_Destroy(gameManager);
    return 0;
}
