#include <stdlib.h>
#include <string.h>
#include "UIManager.h"


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
        if (!uiManager->guiEngine) return UIManager_Destroy(uiManager);
        uiManager->cliEngine = NULL;
    } else {
        uiManager->type = UI_TYPE_CLI;
        uiManager->cliEngine = CLIEngine_Create();
        if (!uiManager->cliEngine) return UIManager_Destroy(uiManager);
        uiManager->guiEngine = NULL;
    }
    return uiManager;
}

UIManager* UIManager_Destroy(UIManager *uiManager) {
    if (!uiManager) return NULL;
    CLIEngine_Destroy(uiManager->cliEngine);
    GUIEngine_Destroy(uiManager->guiEngine);
    free(uiManager);
    return NULL;
}

UIType UIManager_GetUIType(const UIManager *uiManager) {
    if (!uiManager) return UI_TYPE_NONE;
    return uiManager->type;
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

void UIManager_Render(UIManager *uiManager,
                      const GameManager *manager,
                      const GameCommand command) {
    if (!uiManager) return;
    CLIEngine_RenderError(manager, uiManager->type == UI_TYPE_CLI);
    if (uiManager->type == UI_TYPE_GUI) {
        GUIEngine_Render(uiManager->guiEngine, manager, command);
    } else {
        CLIEngine_Render(manager, command);
    }
}
