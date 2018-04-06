#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#include "CLIEngine.h"
#include "GUIEngine.h"
#include "GameManager.h"


typedef struct UIManager UIManager;

UIManager* UIManager_Create(int argc, const char *argv[]);

UIManager* UIManager_Destroy(UIManager *uiManager);

GameCommand UIManager_ProcessInput(UIManager *uiManager);

void UIManager_Render(UIManager *uiManager, const GameManager *manager, GameCommand command);


#endif
