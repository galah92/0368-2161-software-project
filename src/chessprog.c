#include <stdio.h>
#include <string.h>
#include "UIManager.h"
#include "GameManager.h"


int main(int argc, const char *argv[]) {
    // UIManager *uiManager = UIManager_Create(argc, argv);
    (void) argv;
    (void) argc;
    CLIEngine *cliEngine = CLIEngine_Create();
    while (1) {
        GameCommand command = CLIEngine_ProcessInput(cliEngine);
        printf("command type : %u\n",command.type);
        for (int i = 0; i < 8; i++){
            printf(" %d", command.args[i]);
        }
        printf("\n");        
    }
    // GameManager *manager = GameManager_Create();
    // GameCommand command;
    // while (command->type != USER_COMMAND_QUIT) {
    //     switch (manager->currentPlayer->type) {
    //         case PLAYER_TYPE_HUMAN:
    //             command = UIManager_ProcessInput(uiManager);
    //             break;
    //         case PLAYER_TYPE_AI:
    //             command = AIEngine_GetMoveCommand(manager->game);
    //             break;
    //     }
    //     GameManager_ProcessCommand(manager, command);
    //     UIManager_Render(uiManager, manager);
    // }
    // GameManager_Destroy(manager);
    // UIManager_Destroy(uiManager);
    return 0;
}
