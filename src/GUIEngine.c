#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include "GUIEngine.h"


struct GUIEngine {
    int x;
};


GUIEngine* GUIEngine_Create() {
    GUIEngine *this = malloc(sizeof(GUIEngine));
    if (!this) return NULL;
    return this;
}

void GUIEngine_Destroy(GUIEngine *this) {
    if (!this) return;
    free(this);
}

GameCommand GUIEngine_ProcessInput(GUIEngine *this) {
    GameCommand command = { .type = GAME_COMMAND_INVALID };
    if (!this) return command;
    return command;
}

void GUIEngine_Render(GUIEngine *this, const GameManager *manager, GameCommand command) {
    if (!manager) return;
    printf("%d", this->x);
    (void) command;
}
