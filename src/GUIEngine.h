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
 */
void GUIEngine_Destroy(GUIEngine *engine);

/**
 * Get and parse user input from stdin.
 * @param   engine      the instance to use
 * @return  command     type is GAME_COMMAND_INVALID if engine is null
 *                      a command and args as parsed from CLI otherwise
 */
GameCommand GUIEngine_ProcessInput(GUIEngine *engine);

/**
 * Output to CLI the current game state.
 * @param   engine      the instance to use
 * @param   gameState   the gameState to output
 */
void GUIEngine_Render(GUIEngine *engine, const GameState *gameState);


#endif
