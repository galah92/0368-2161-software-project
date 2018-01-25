#include <stdio.h>
#include "UIManager.h"
#include "GameCommand.h"
// #include "GameManager.h"
#include <SDL.h>


int main(int argc, const char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init() error: %s\n", SDL_GetError());
        return 1;
    }
    return 0;
    UIManager *uiManager = UIManager_Create(argc, argv);
    // GameManager *gameManager = GameManager_Create();
    // GameCommand command;
    // while (command->type != USER_COMMAND_QUIT) {
    //     switch (gameManager->currentPlayer->type) {
    //         case PLAYER_TYPE_HUMAN:
    //             command = UIManager_ProcessInput(uiManager);
    //             break;
    //         case PLAYER_TYPE_AI:
    //             command = AIEngine_GetMoveCommand(gameManager->game);
    //             break;
    //     }
    //     GameManager_ProcessCommand(gameManager, command);
    //     UIManager_Render(uiManager, gameManager->gameState);
    // }
    // GameManager_Destroy(gameManager);
    UIManager_Destroy(uiManager);
    return 0;
}
