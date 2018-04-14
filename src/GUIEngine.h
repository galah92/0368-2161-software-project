#ifndef GUI_ENGINE_H_
#define GUI_ENGINE_H_

#include "GameManager.h"


typedef struct GUIEngine GUIEngine;

/**
 * Create new GUIEngine instance.
 * @return  NULL if malloc failed
 *          GUIEngine* instance otherwise
 */
GUIEngine* GUIEngine_Create();

/**
 * Free all resources for a given GUIEngine instance.
 * @param   engine      the instance to destroy
 * @return  NULL
 */
GUIEngine* GUIEngine_Destroy(GUIEngine *engine);

/**
 * Get and parse user input from given GUIEngine instance.
 * @param   engine      the instance to use
 * @return  command     a command and args
 */
GameCommand GUIEngine_ProcessInput(GUIEngine *engine);

/**
 * Output to CLI the current game state.
 * @param   engine      the instance to use
 * @param   gameManager the game state to render
 * @param   command     the last command processed
 */
void GUIEngine_Render(GUIEngine *engine,
                      const GameManager *manager,
                      const GameCommand command);


#endif
