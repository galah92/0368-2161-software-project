#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "GUIEngine.h"


struct GUIEngine {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
};

GUIEngine* GUIEngine_Create() {
    GUIEngine *engine = malloc(sizeof(GUIEngine));
    if (!engine) return NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return GUIEngine_Destroy(engine);
    engine->window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);
    if (!engine->window) return GUIEngine_Destroy(engine);
    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_SOFTWARE);
    if (!engine->renderer) return GUIEngine_Destroy(engine);

    SDL_Surface* boardSurface = SDL_LoadBMP("./gui/chessboard.bmp");
	if (!boardSurface) return GUIEngine_Destroy(engine);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(engine->renderer, boardSurface);
	if (!tex) return GUIEngine_Destroy(engine);
	SDL_FreeSurface(boardSurface);
    SDL_DestroyTexture(tex);
    SDL_RenderCopy(engine->renderer, tex, NULL, NULL);
    SDL_RenderPresent(engine->renderer);

    return engine;
}

GUIEngine* GUIEngine_Destroy(GUIEngine *engine) {
    const char *error = SDL_GetError();
    if (strcmp(error, "")) printf("SDL Error: %s\n", error);
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
    while (true) {
        SDL_WaitEvent(&engine->event);
        switch (engine->event.type) {
            case SDL_WINDOWEVENT_CLOSE:
            case SDL_QUIT:
                command.type = GAME_COMMAND_QUIT;
                return command;
            case SDL_WINDOWEVENT:
                if (engine->event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    command.type = GAME_COMMAND_QUIT;
                    return command;
                }
                break;
        }
    }
}

void GUIEngine_Render(GUIEngine *engine, const GameManager *manager, GameCommand command) {
    if (!manager) return;
    (void)engine;
    (void)command;
}
