#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
#include <SDL_video.h>
#include "GUIEngine.h"
#include "GUIUtils.h"
#include "GUIBoard.h"


#define WINDOW_W                1024
#define WINDOW_H                768
#define BUTTON_W                200
#define BUTTON_H                50

#define SDL_ERROR_STRING        "SDL Error: %s\n"
#define SRC_BACKGROUND          "./gui/chessboard.bmp"

#define SRC_BUTTON_NEW_GAME     "./gui/colors/newgame.bmp"
#define SRC_BUTTON_LOAD         "./gui/colors/load.bmp"
#define SRC_BUTTON_QUIT         "./gui/colors/quit.bmp"

#define SRC_BUTTON_BACK         "./gui/colors/back.bmp"
#define SRC_BUTTON_GAME_MODE    "./gui/colors/gamemode.bmp"
#define SRC_BUTTON_DIFFICULTY   "./gui/colors/difficulty.bmp"
#define SRC_BUTTON_USER_COLOR   "./gui/colors/usercolor.bmp"
#define SRC_BUTTON_START        "./gui/colors/start.bmp"

#define SRC_BUTTON_RESTART      "./gui/colors/restart.bmp"
#define SRC_BUTTON_SAVE         "./gui/colors/save.bmp"
#define SRC_BUTTON_UNDO         "./gui/colors/undo.bmp"
#define SRC_BUTTON_MAIN_MENU    "./gui/colors/mainmenu.bmp"

#define SRC_BUTTON_SLOT1        "./gui/colors/slot01.bmp"
#define SRC_BUTTON_SLOT2        "./gui/colors/slot02.bmp"
#define SRC_BUTTON_SLOT3        "./gui/colors/slot03.bmp"
#define SRC_BUTTON_SLOT4        "./gui/colors/slot04.bmp"
#define SRC_BUTTON_SLOT5        "./gui/colors/slot05.bmp"


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
    Pane *pane;
    Board *board;
};

typedef enum GUICommandType {
    GUI_COMMAND_NONE,
    GUI_COMMAND_SWITCH_PANE,
    GUI_COMMAND_GAME_COMMAND,
    GUI_COMMAND_BOTH,
} GUICommandType;

typedef struct GUICommand {
    GUICommandType type;
    PaneType paneType;
    GameCommand gameCommand;
} GUICommand;

void* handleNewGameButton(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->type = GUI_COMMAND_SWITCH_PANE;
    guiCommand->paneType = PANE_TYPE_SETTINGS;
    return NULL;
}

void* handleLoadPaneButton(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->type = GUI_COMMAND_SWITCH_PANE;
    guiCommand->paneType = PANE_TYPE_LOAD;
    return NULL;
}

void* handleQuitButton(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_QUIT;
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}

void* handleBackButton(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_RESET;
    guiCommand->paneType = PANE_TYPE_MAIN;
    guiCommand->type = GUI_COMMAND_BOTH;
    return NULL;
}

void* handleStartButton(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_START;
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}

void* handleRestartButton(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_RESET;
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}

void* handleLoadButton(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_LOAD_GAME;
    strcpy(guiCommand->gameCommand.path, "slot1.save");
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}

void* handleSlot1Button(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_LOAD_GAME;
    strcpy(guiCommand->gameCommand.path, "slot1.save");
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}
void* handleSlot2Button(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_LOAD_GAME;
    strcpy(guiCommand->gameCommand.path, "slot2.save");
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}
void* handleSlot3Button(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_LOAD_GAME;
    strcpy(guiCommand->gameCommand.path, "slot3.save");
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}
void* handleSlot4Button(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_LOAD_GAME;
    strcpy(guiCommand->gameCommand.path, "slot4.save");
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}
void* handleSlot5Button(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_LOAD_GAME;
    strcpy(guiCommand->gameCommand.path, "slot5.save");
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}

void* handleUndoButton(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_UNDO;
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}

void *handleBoardEvent(BoardEventArgs *event, void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_MOVE;
    memcpy(guiCommand->gameCommand.args, event->move, sizeof(int) * GUI_BOARD_MOVE_ARGS);
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}

Pane* MainPane_Create(SDL_Renderer *renderer) {
    Button *buttons[] = {
        Button_Create(renderer,
                      SRC_BUTTON_NEW_GAME,
                      (SDL_Rect){ .x = 25, .y = 25, .w = BUTTON_W, .h = BUTTON_H },
                      handleNewGameButton),
        Button_Create(renderer,
                      SRC_BUTTON_LOAD,
                      (SDL_Rect){ .x = 25, .y = 100, .w = BUTTON_W, .h = BUTTON_H },
                      handleLoadPaneButton),
        Button_Create(renderer,
                      SRC_BUTTON_QUIT,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      handleQuitButton),
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
                      handleBackButton),
        Button_Create(renderer,
                      SRC_BUTTON_START,
                      (SDL_Rect){ .x = 25, .y = 325, .w = BUTTON_W, .h = BUTTON_H },
                      handleStartButton),
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
                      handleBackButton),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT1,
                      (SDL_Rect){ .x = 25, .y = 100, .w = BUTTON_W, .h = BUTTON_H },
                      handleSlot1Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT2,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      handleSlot2Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT3,
                      (SDL_Rect){ .x = 25, .y = 250, .w = BUTTON_W, .h = BUTTON_H },
                      handleSlot3Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT4,
                      (SDL_Rect){ .x = 25, .y = 325, .w = BUTTON_W, .h = BUTTON_H },
                      handleSlot4Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT5,
                      (SDL_Rect){ .x = 25, .y = 400, .w = BUTTON_W, .h = BUTTON_H },
                      handleSlot5Button),
        Button_Create(renderer,
                      SRC_BUTTON_LOAD,
                      (SDL_Rect){ .x = 25, .y = 475, .w = BUTTON_W, .h = BUTTON_H },
                      handleLoadButton),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       7,
                       NULL);
}

Pane* GamePane_Create(SDL_Renderer *renderer) {
    Button *buttons[] = {
        Button_Create(renderer,
                      SRC_BUTTON_RESTART,
                      (SDL_Rect){ .x = 25, .y = 25, .w = BUTTON_W, .h = BUTTON_H },
                      handleRestartButton),
        Button_Create(renderer,
                      SRC_BUTTON_SAVE,
                      (SDL_Rect){ .x = 25, .y = 100, .w = BUTTON_W, .h = BUTTON_H },
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_LOAD,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      handleLoadPaneButton),
        Button_Create(renderer,
                      SRC_BUTTON_UNDO,
                      (SDL_Rect){ .x = 25, .y = 250, .w = BUTTON_W, .h = BUTTON_H },
                      handleUndoButton),
        Button_Create(renderer,
                      SRC_BUTTON_MAIN_MENU,
                      (SDL_Rect){ .x = 25, .y = 325, .w = BUTTON_W, .h = BUTTON_H },
                      handleBackButton),
        Button_Create(renderer,
                      SRC_BUTTON_QUIT,
                      (SDL_Rect){ .x = 25, .y = 400, .w = BUTTON_W, .h = BUTTON_H },
                      handleQuitButton),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       6,
                       NULL);
}

void pseudoRender(GUIEngine *engine, const ChessGame *game) {
    SDL_RenderClear(engine->renderer);
    SDL_RenderCopy(engine->renderer, engine->bgTexture, NULL, NULL);
    Pane_Render(engine->pane);
    Board_Render(engine->board, game);
    SDL_RenderPresent(engine->renderer);
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
    SDL_SetRenderDrawBlendMode(engine->renderer, SDL_BLENDMODE_BLEND);
    SDL_Surface* boardSurface = SDL_LoadBMP(SRC_BACKGROUND);
    if (!boardSurface) return GUIEngine_Destroy(engine);
    engine->bgTexture = SDL_CreateTextureFromSurface(engine->renderer, boardSurface);
    if (!engine->bgTexture) return GUIEngine_Destroy(engine);
    SDL_FreeSurface(boardSurface);
    engine->paneType = PANE_TYPE_MAIN;
    engine->pane = MainPane_Create(engine->renderer);
    engine->board = Board_Create(engine->renderer, handleBoardEvent);
    pseudoRender(engine, NULL);
    return engine;
}

GUIEngine* GUIEngine_Destroy(GUIEngine *engine) {
    if (!engine) return NULL;
    const char *error = SDL_GetError();
    if (strcmp(error, "")) printf(SDL_ERROR_STRING, error);
    Pane_Destroy(engine->pane);
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

void updatePane(GUIEngine *engine, PaneType type) {
    switch (type) {
        case PANE_TYPE_MAIN:
            Pane_Destroy(engine->pane);
            engine->pane = MainPane_Create(engine->renderer);
            engine->paneType = PANE_TYPE_MAIN;
            break;
        case PANE_TYPE_SETTINGS:
            Pane_Destroy(engine->pane);
            engine->pane = SettingsPane_Create(engine->renderer);
            engine->paneType = PANE_TYPE_SETTINGS;
            break;
        case PANE_TYPE_LOAD:
            Pane_Destroy(engine->pane);
            engine->pane = LoadPane_Create(engine->renderer);
            engine->paneType = PANE_TYPE_LOAD;
            break;
        case PANE_TYPE_GAME:
            Pane_Destroy(engine->pane);
            engine->pane = GamePane_Create(engine->renderer);
            engine->paneType = PANE_TYPE_GAME;
            break;
        default:
            break;
    }
}

GameCommand GUIEngine_ProcessInput(GUIEngine *engine) {
    GUICommand guiCommand = { .type = GUI_COMMAND_NONE };
    guiCommand.gameCommand = (GameCommand){ .type = GAME_COMMAND_INVALID };
    if (!engine) return guiCommand.gameCommand;
    while (true) {
        SDL_WaitEvent(&engine->event);
        if (isExitButtonEvent(&engine->event)) {
            guiCommand.gameCommand.type = GAME_COMMAND_QUIT;
            return guiCommand.gameCommand;
        }
        Pane_HandleEvent(engine->pane, &engine->event, &guiCommand);
        Board_HandleEvent(engine->board, &engine->event, &guiCommand);
        pseudoRender(engine, NULL);
        switch (guiCommand.type) {
            case GUI_COMMAND_GAME_COMMAND:
                return guiCommand.gameCommand;
            case GUI_COMMAND_SWITCH_PANE:
                updatePane(engine, guiCommand.paneType);
                break;
            case GUI_COMMAND_BOTH:
                updatePane(engine, guiCommand.paneType);
                return guiCommand.gameCommand;
            default:
                break;
        }
    }
}

void GUIEngine_Render(GUIEngine *engine, const GameManager *manager, GameCommand command) {
    if (!manager) return;
    switch (manager->error) {
        case GAME_ERROR_NONE:
            break;
        case GAME_ERROR_INVALID_COMMAND:
        case GAME_ERROR_INVALID_GAME_MODE:
        case GAME_ERROR_INVALID_DIFF_LEVEL:
        case GAME_ERROR_INVALID_USER_COLOR:
        case GAME_ERROR_INVALID_FILE:
        case GAME_ERROR_INVALID_POSITION:
        case GAME_ERROR_EMPTY_POSITION:
        case GAME_ERROR_INVALID_MOVE:
        case GAME_ERROR_INVALID_MOVE_KING_IS_T:
        case GAME_ERROR_INVALID_MOVE_KING_WILL_T:
        case GAME_ERROR_FILE_ALLOC:
        case GAME_ERROR_EMPTY_HISTORY:
            return;
    }
    switch (command.type) {
        case GAME_COMMAND_GAME_MODE:
        case GAME_COMMAND_DIFFICULTY:
        case GAME_COMMAND_USER_COLOR:
        case GAME_COMMAND_LOAD_GAME:
            updatePane(engine, PANE_TYPE_GAME);
            break;
        case GAME_COMMAND_DEFAULT_SETTINGS:
        case GAME_COMMAND_PRINT_SETTINGS:
        case GAME_COMMAND_START:
            updatePane(engine, PANE_TYPE_GAME);
            break;
        case GAME_COMMAND_MOVE:
        case GAME_COMMAND_GET_MOVES:
        case GAME_COMMAND_SAVE:
        case GAME_COMMAND_UNDO:
        case GAME_COMMAND_RESET:
        case GAME_COMMAND_QUIT:
        case GAME_COMMAND_INVALID:
            break;
    }
    pseudoRender(engine, manager->game);
}
