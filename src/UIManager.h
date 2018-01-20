#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#include "CLIEngine.h"
#include "GameState.h"
#include "UserCommand.h"


typedef struct UIManager_t UIManager;

UIManager* UIManager_Create(int argc, char *argv[]);

void UIManager_Destroy(UIManager *manager);

UserCommand* UIManager_ProcessInput(UIManager *manager);

void UIManager_Render(UIManager *manager, GameState *gameState);


#endif
