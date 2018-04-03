#include <string.h>
#include "UIManager.h"


typedef enum UIType {
    UI_TYPE_CLI,
    UI_TYPE_GUI,
} UIType;

struct UIManager {
    UIType type;
    CLIEngine *cliEngine;
    GUIEngine *guiEngine;
};

UIManager* UIManager_Create(int argc, const char *argv[]) {
    UIManager *uiManager = malloc(sizeof(UIManager));
    if (!uiManager) return NULL;
    if (argc > 1 && strcmp(argv[1], "-g") == 0) {
        uiManager->type = UI_TYPE_GUI;
        uiManager->guiEngine = GUIEngine_Create();
    } else {
        uiManager->type = UI_TYPE_CLI;
    }
    uiManager->cliEngine = CLIEngine_Create();  // we need CLI anyway
    if (!uiManager->cliEngine) UIManager_Destroy(uiManager);
    return uiManager;
}

void UIManager_Destroy(UIManager *uiManager) {
    if (!uiManager) return;
    CLIEngine_Destroy(uiManager->cliEngine);
    GUIEngine_Destroy(uiManager->guiEngine);
    free(uiManager);
}

GameCommand UIManager_ProcessInput(UIManager *uiManager) {
    GameCommand command = { .type = GAME_COMMAND_INVALID };
    if (!uiManager) return command;
    switch (uiManager->type) {
        case UI_TYPE_GUI:    
            return GUIEngine_ProcessInput(uiManager->guiEngine);
        case UI_TYPE_CLI:
        default:
            return CLIEngine_ProcessInput(uiManager->cliEngine);
    }
}

void UIManager_Render(UIManager *uiManager, const GameManager *manager, GameCommand command) {
    if (!uiManager) return;
    if (uiManager->type == UI_TYPE_GUI) {
        GUIEngine_Render(uiManager->guiEngine, manager, command);
    }
    CLIEngine_Render(uiManager->cliEngine, manager, command);
}
