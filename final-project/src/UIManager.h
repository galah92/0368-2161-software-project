#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#include "CLIEngine.h"
#include "GUIEngine.h"
#include "GameManager.h"


typedef enum UIType {
    UI_TYPE_NONE,
    UI_TYPE_CLI,
    UI_TYPE_GUI,
} UIType;

typedef struct UIManager UIManager;

/**
 * Create new UIEngine instance, using CLIEngine or GUIEngine based on
 * the command-line arguments.
 * @param   argc        number of command-line arguments
 * @param   argv        the command-line arguments
 * @return  NULL if malloc failed
 *          UIEngine* instance otherwise
 */
UIManager* UIManager_Create(int argc, const char *argv[]);

/**
 * Free all resources for a given UIEngine instance.
 * @param   engine      the instance to destroy
 * @return  NULL
 */
UIManager* UIManager_Destroy(UIManager *uiManager);

/**
 * Retrieve a given UIManager's inner UIType
 * @param   engine      the instance to retrive it's UIType
 * @return  type        the UIType if uiManager != NULL
 *          UI_TYPE_NONE otherwise
 */
UIType UIManager_GetUIType(const UIManager *uiManager);

/**
 * Get and parse user input.
 * @param   engine      the UIManager instance to use
 * @return  command     a command and args
 */
GameCommand UIManager_ProcessInput(UIManager *uiManager);

/**
 * Output current game state.
 * @param   uiManager   the UIManager instance to use
 * @param   gameManager the game state to render
 * @param   command     the last command processed
 */
void UIManager_Render(UIManager *uiManager,
                      const GameManager *gameManager,
                      const GameCommand command);


#endif
