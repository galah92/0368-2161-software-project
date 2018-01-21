#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#include "CLIEngine.h"
#include "GameState.h"
#include "GameCommand.h"


typedef struct UIManager_t UIManager;

UIManager* UIManager_Create(int argc, const char *argv[]);

void UIManager_Destroy(UIManager *manager);

GameCommand UIManager_ProcessInput(UIManager *manager);

void UIManager_Render(UIManager *manager, const GameState *gameState);


#endif
