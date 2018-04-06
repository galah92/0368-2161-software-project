#include <unistd.h>
#include <stdio.h>
#include <SDL.h>
#include "GUIEngine.h"


struct GUIEngine {
    SDL_Window *window;
    SDL_Renderer *renderer;
};


GUIEngine* GUIEngine_Create() {
    GUIEngine *engine = malloc(sizeof(GUIEngine));
    if (!engine) return NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
        GUIEngine_Destroy(engine);
        return NULL;
	}
    engine->window = SDL_CreateWindow("Chess",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      800, 600, SDL_WINDOW_OPENGL);
    return engine;
}

void GUIEngine_Destroy(GUIEngine *engine) {
    if (!engine) return;
    SDL_DestroyWindow(engine->window);
    free(engine);
}

GameCommand GUIEngine_ProcessInput(GUIEngine *engine) {
    GameCommand command = { .type = GAME_COMMAND_INVALID };
    if (!engine) return command;
    sleep(10);
    command.type = GAME_COMMAND_QUIT;
    return command;
}

void GUIEngine_Render(GUIEngine *engine, const GameManager *manager, GameCommand command) {
    if (!manager) return;
    (void)engine;
    (void)command;
}
