#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>
#include "GUIEngine.h"
#include "GUIUtils.h"


#define WINDOW_W                1024
#define WINDOW_H                768
#define BUTTON_W                200
#define BUTTON_H                50

#define SDL_ERROR_STRING        "SDL Error: %s\n"
#define SRC_BACKGROUND          "./gui/chessboard.bmp"

#define SRC_BUTTON_NEW_GAME     "./gui/colors/black.bmp"
#define SRC_BUTTON_LOAD         "./gui/colors/black.bmp"
#define SRC_BUTTON_QUIT         "./gui/colors/black.bmp"

#define SRC_BUTTON_BACK         "./gui/colors/blue.bmp"
#define SRC_BUTTON_GAME_MODE    "./gui/colors/blue.bmp"
#define SRC_BUTTON_DIFFICULTY   "./gui/colors/blue.bmp"
#define SRC_BUTTON_USER_COLOR   "./gui/colors/blue.bmp"
#define SRC_BUTTON_START        "./gui/colors/blue.bmp"

#define SRC_BUTTON_RESTART      "./gui/colors/green.bmp"
#define SRC_BUTTON_SAVE         "./gui/colors/green.bmp"
#define SRC_BUTTON_UNDO         "./gui/colors/green.bmp"
#define SRC_BUTTON_MAIN_MENU    "./gui/colors/green.bmp"

#define SRC_BUTTON_SLOT1        "./gui/colors/yellow.bmp"
#define SRC_BUTTON_SLOT2        "./gui/colors/yellow.bmp"
#define SRC_BUTTON_SLOT3        "./gui/colors/yellow.bmp"
#define SRC_BUTTON_SLOT4        "./gui/colors/yellow.bmp"
#define SRC_BUTTON_SLOT5        "./gui/colors/yellow.bmp"


typedef enum PaneType {
    PANE_TYPE_MAIN,
    PANE_TYPE_SETTINGS,
    PANE_TYPE_GAME,
    PANE_TYPE_LOAD,
} PaneType;

struct GUIEngine {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *bgTexture;
    SDL_Event event;
    PaneType paneType;
    PaneType backPaneType;
    Pane *mainPane;
    Pane *settingsPane;
    Pane *loadPane;
};

void pseudoRender(GUIEngine *engine) {
    SDL_RenderClear(engine->renderer);
    SDL_RenderCopy(engine->renderer, engine->bgTexture, NULL, NULL);
    switch (engine->paneType) {
        case PANE_TYPE_MAIN:
            Pane_Render(engine->mainPane);
            break;
        case PANE_TYPE_SETTINGS:
            Pane_Render(engine->settingsPane);
            break;
        case PANE_TYPE_LOAD:
            Pane_Render(engine->loadPane);
            break;
        default:
            break;
    }
    SDL_RenderPresent(engine->renderer);
}

Pane* MainPane_Create(SDL_Renderer *renderer) {
    Button *buttons[] = {
        Button_Create(renderer,
                      SRC_BUTTON_NEW_GAME,
                      (SDL_Rect){ .x = 25, .y = 25, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_LOAD,
                      (SDL_Rect){ .x = 25, .y = 100, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_QUIT,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       3,
                       NULL);
}

Pane* SettingsPane_Create(SDL_Renderer *renderer) {
    Button *buttons[] = {
        Button_Create(renderer,
                      SRC_BUTTON_GAME_MODE,
                      (SDL_Rect){ .x = 25, .y = 25, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_DIFFICULTY,
                      (SDL_Rect){ .x = 25, .y = 100, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_USER_COLOR,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_BACK,
                      (SDL_Rect){ .x = 25, .y = 250, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_START,
                      (SDL_Rect){ .x = 25, .y = 325, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       5,
                       NULL);
}

Pane* LoadPane_Create(SDL_Renderer *renderer) {
    Button *buttons[] = {
        Button_Create(renderer,
                      SRC_BUTTON_BACK,
                      (SDL_Rect){ .x = 25, .y = 25, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT1,
                      (SDL_Rect){ .x = 25, .y = 100, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT2,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT3,
                      (SDL_Rect){ .x = 25, .y = 250, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT4,
                      (SDL_Rect){ .x = 25, .y = 325, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT5,
                      (SDL_Rect){ .x = 25, .y = 400, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_LOAD,
                      (SDL_Rect){ .x = 25, .y = 475, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       7,
                       NULL);
}

GUIEngine* GUIEngine_Create() {
    GUIEngine *engine = malloc(sizeof(GUIEngine));
    if (!engine) return NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return GUIEngine_Destroy(engine);
    engine->window = SDL_CreateWindow("Chess",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      WINDOW_W,
                                      WINDOW_H,
                                      SDL_WINDOW_OPENGL);
    if (!engine->window) return GUIEngine_Destroy(engine);
    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_SOFTWARE);
    if (!engine->renderer) return GUIEngine_Destroy(engine);
    SDL_Surface* boardSurface = SDL_LoadBMP(SRC_BACKGROUND);
	if (!boardSurface) return GUIEngine_Destroy(engine);
	engine->bgTexture = SDL_CreateTextureFromSurface(engine->renderer, boardSurface);
	if (!engine->bgTexture) return GUIEngine_Destroy(engine);
	SDL_FreeSurface(boardSurface);
    engine->mainPane = MainPane_Create(engine->renderer);
    engine->settingsPane = SettingsPane_Create(engine->renderer);
    engine->loadPane = LoadPane_Create(engine->renderer);
    engine->paneType = PANE_TYPE_LOAD;
    pseudoRender(engine);
    return engine;
}

GUIEngine* GUIEngine_Destroy(GUIEngine *engine) {
    const char *error = SDL_GetError();
    if (strcmp(error, "")) printf(SDL_ERROR_STRING, error);
    if (!engine) return NULL;
    Pane_Destroy(engine->mainPane);
    Pane_Destroy(engine->settingsPane);
    Pane_Destroy(engine->loadPane);
    if (!engine->bgTexture) SDL_DestroyTexture(engine->bgTexture);
    if (!engine->renderer) SDL_DestroyRenderer(engine->renderer);
    if (!engine->window) SDL_DestroyWindow(engine->window);
    SDL_Quit();
    free(engine);
    return NULL;
}

bool isExitButtonEvent(SDL_Event *event) {
    switch (event->type) {
        case SDL_WINDOWEVENT_CLOSE:
        case SDL_QUIT:;
            return true;
        case SDL_WINDOWEVENT:
            if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
                return true;
            }
            break;
    }
    return false;
}

typedef enum GUICommandType {
    GUI_COMMAND_NONE,
    GUI_COMMAND_MAIN_PANE,
    GUI_COMMAND_NEW_GAME_PANE,
    GUI_COMMAND_LOAD_PANE,
    GUI_COMMAND_GAME_COMMAND,
} GUICommandType;

GameCommand GUIEngine_ProcessInput(GUIEngine *engine) {
    GameCommand command = { .type = GAME_COMMAND_INVALID };
    if (!engine) return command;
    // GUICommandType guiCommand = GUI_COMMAND_NONE;
    while (true) {
        SDL_WaitEvent(&engine->event);
        if (isExitButtonEvent(&engine->event)) {
            command.type = GAME_COMMAND_QUIT;
            return command;
        }
        switch (engine->paneType) {
            case PANE_TYPE_MAIN:
                Pane_HandleEvent(engine->mainPane, &engine->event);
                break;
            case PANE_TYPE_SETTINGS:
                Pane_HandleEvent(engine->settingsPane, &engine->event);
                break;
            case PANE_TYPE_LOAD:
                Pane_HandleEvent(engine->loadPane, &engine->event);
                break;
            default:
                break;
        }
        pseudoRender(engine);
    }
}

void GUIEngine_Render(GUIEngine *engine, const GameManager *manager, GameCommand command) {
    if (!manager) return;
    (void)command;
    pseudoRender(engine);
}
