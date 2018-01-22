#include <string.h>
#include "UIManager.h"


typedef enum UIType {
    UI_TYPE_CLI,
    UI_TYPE_GUI,
} UIType;

struct UIManager {
    UIType type;
    CLIEngine *cliEngine;
    // GUIEngine *guiEngine;
};

UIManager* UIManager_Create(int argc, const char *argv[]) {
    UIManager *this = malloc(sizeof(UIManager));
    if (!this) return NULL;
    if (argc > 1 && strcmp(argv[1], "-g") == 0) {
        this->type = UI_TYPE_GUI;
        // this->guiEngine = GUIEngine_Create();
    } else {
        this->type = UI_TYPE_CLI;
    }
    this->cliEngine = CLIEngine_Create();  // we need CLI anyway
    if (!this->cliEngine) UIManager_Destroy(this);
    return this;
}

void UIManager_Destroy(UIManager *this) {
    if (!this) return;
    CLIEngine_Destroy(this->cliEngine);
    // GUIEngine_Destroy(this->guiEngine);
    free(this);
}

GameCommand UIManager_ProcessInput(UIManager *this) {
    GameCommand command = { .type = GAME_COMMAND_INVALID };
    if (!this) return command;
    switch (this->type) {
        case UI_TYPE_GUI:    
            // return GUIEngine_ProcessInput(this->guiEngine);
            return CLIEngine_ProcessInput(this->cliEngine);
        case UI_TYPE_CLI:
        default:
            return CLIEngine_ProcessInput(this->cliEngine);
    }
}

void UIManager_Render(UIManager *this, const GameState *gameState) {
    if (!this) return;
    if (this->type == UI_TYPE_GUI) {
        // GUIEngine_Render(this->guiEngine, gameState);
    }
    CLIEngine_Render(this->cliEngine, gameState);
}
