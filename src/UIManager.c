#include <string.h>
#include "UIManager.h"


struct UIManager_t {
    enum { CLI_ENGINE, GUI_ENGINE } type;
    CLIEngine *cliEngine;
    // GUIEngine *guiEngine;
};

UIManager* UIManager_Create(int argc, const char *argv[]) {
    UIManager *manager = malloc(sizeof(UIManager));
    if (!manager) return NULL;
    if (argc > 1 && strcmp(argv[1], "-g") == 0) {
        manager->type = GUI_ENGINE;
        // manager->guiEngine = GUIEngine_Create();
    }
    manager->cliEngine = CLIEngine_Create();
    if (!manager->cliEngine) UIManager_Destroy(manager);
    return manager;
}

void UIManager_Destroy(UIManager *manager) {
    if (!manager) return;
    CLIEngine_Destroy(manager->cliEngine);
    // GUIEngine_Destroy(manager->guiEngine);
    free(manager);
}

UserCommand* UIManager_ProcessInput(UIManager *manager) {
    if (!manager) return NULL;
    if (manager->type == GUI_ENGINE) {
        // return GUIEngine_ProcessInput(manager->guiEngine);
        return CLIEngine_ProcessInput(manager->cliEngine);
    } else {
        return CLIEngine_ProcessInput(manager->cliEngine);
    }
}

void UIManager_Render(UIManager *manager, const GameState *gameState) {
    if (!manager) return;
    CLIEngine_Render(manager->cliEngine, gameState);
    if (manager->type == GUI_ENGINE) {
        // GUIEngine_Render(manager->guiEngine, gameState);
    }
}
