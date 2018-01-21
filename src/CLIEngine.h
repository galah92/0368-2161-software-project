#ifndef CLI_ENGINE_H_
#define CLI_ENGINE_H_

#include "GameState.h"
#include "GameCommand.h"


typedef struct CLIEngine_t {
    int _x;  // TODO: consider removing, as there's no need for a struct here (no state)
} CLIEngine;

CLIEngine* CLIEngine_Create();

void CLIEngine_Destroy(CLIEngine *engine);

GameCommand CLIEngine_ProcessInput(CLIEngine *engine);

void CLIEngine_Render(CLIEngine *engine, const GameState *gameState);


#endif
