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
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return GUIEngine_Destroy(engine);
    engine->window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);
    if (!engine->window) return GUIEngine_Destroy(engine);
    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_SOFTWARE);
    if (!engine->renderer) return GUIEngine_Destroy(engine);
    return engine;
}

GUIEngine* GUIEngine_Destroy(GUIEngine *engine) {
    const char *error = SDL_GetError();
    if (strcmp(error, "") == 0) printf("SDL Error: %s\n", error);
    if (!engine) return NULL;
    if (!engine->renderer) SDL_DestroyRenderer(engine->renderer);
    if (!engine->window) SDL_DestroyWindow(engine->window);
    SDL_Quit();
    free(engine);
    return NULL;
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
