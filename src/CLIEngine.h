#ifndef CLI_ENGINE_H_
#define CLI_ENGINE_H_

#include "GameState.h"
#include "GameCommand.h"


typedef struct CLIEngine_t CLIEngine;

/**
 * Create new CLIEngine instance.
 * @return  NULL if malloc failed
 *          CLIEngine* instance otherwise
 */
CLIEngine* CLIEngine_Create();

/**
 * Free all resources for a given CLIEngine instance.
 * @param   engine      the instance to destroy
 */
void CLIEngine_Destroy(CLIEngine *engine);

/**
 * Get and parse user input from stdin.
 * @param   engine      the instance to use
 * @return  command     type is GAME_COMMAND_INVALID if engine is null
 *                      a command and args as parsed from CLI otherwise
 */
GameCommand CLIEngine_ProcessInput(CLIEngine *engine);

/**
 * Output to CLI the current game state.
 * @param   engine      the instance to use
 * @param   gameState   the gameState to output
 */
void CLIEngine_Render(CLIEngine *engine, const GameState *gameState);


#endif
