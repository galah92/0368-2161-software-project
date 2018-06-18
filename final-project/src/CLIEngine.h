#ifndef CLI_ENGINE_H_
#define CLI_ENGINE_H_

#include <stdbool.h>
#include "GameManager.h"


typedef struct CLIEngine CLIEngine;

/**
 * Create new CLIEngine instance.
 * @return  NULL if malloc failed
 *          CLIEngine* instance otherwise
 */
CLIEngine* CLIEngine_Create();

/**
 * Free all resources for a given CLIEngine instance.
 * @param   engine      the instance to destroy
 * @return  NULL
 */
CLIEngine* CLIEngine_Destroy(CLIEngine *engine);

/**
 * Get and parse user input from stdin.
 * @param   engine      the instance to use
 * @return  command     type is GAME_COMMAND_INVALID if engine is null
 *                      a command and args as parsed from CLI otherwise
 */
GameCommand CLIEngine_ProcessInput(CLIEngine *engine);

/**
 * Output to CLI an error if one exists.
 * @param   engine      the instance to use
 * @param   gameManager the game state's error to output
 */
void CLIEngine_RenderError(const GameManager *manager, bool toRenderEnterMove);

/**
 * Output to CLI the current game state.
 * @param   engine      the instance to use
 * @param   gameManager the game state to output
 * @param   command     the last command processed
 */
void CLIEngine_Render(const GameManager *manager, const GameCommand command);


#endif
