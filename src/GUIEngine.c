#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
#include <SDL_video.h>
#include "GUIEngine.h"
#include "GUIUtils.h"
#include "GUIBoard.h"
#include "ChessGame.h"
#include "ArrayStack.h"


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
    Pane *pane;
    Board *board;
    unsigned int slot;
};

typedef enum GUICommandType {
    GUI_COMMAND_NONE,
    GUI_COMMAND_SWITCH_PANE,
    GUI_COMMAND_SET_SLOT,
    GUI_COMMAND_GAME_COMMAND,
    GUI_COMMAND_BOTH,
} GUICommandType;

typedef struct GUICommand {
    GUICommandType type;
    PaneType paneType;
    GameCommand gameCommand;
    unsigned int slot;
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
    guiCommand->gameCommand.type = GAME_COMMAND_RESTART;
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}

char* slotToPath(unsigned int slot) {
    switch (slot) {
        case 1: return ".slot1.save";
        case 2: return ".slot2.save";
        case 3: return ".slot3.save";
        case 4: return ".slot4.save";
        case 5: return ".slot5.save";
        default: return ".slot1.save"; // shouldn't happen
    }
}

void* handleLoadButton(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_LOAD_GAME;
    strcpy(guiCommand->gameCommand.path, slotToPath(guiCommand->slot));
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}

void* handleSaveButton(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_SAVE;
    strcpy(guiCommand->gameCommand.path, slotToPath(guiCommand->slot));
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}

void* handleSlot1Button(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->type = GUI_COMMAND_SET_SLOT;
    guiCommand->slot = 1;
    return NULL;
}
void* handleSlot2Button(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->type = GUI_COMMAND_SET_SLOT;
    guiCommand->slot = 2;
    return NULL;
}
void* handleSlot3Button(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->type = GUI_COMMAND_SET_SLOT;
    guiCommand->slot = 3;
    return NULL;
}
void* handleSlot4Button(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->type = GUI_COMMAND_SET_SLOT;
    guiCommand->slot = 4;
    return NULL;
}
void* handleSlot5Button(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->type = GUI_COMMAND_SET_SLOT;
    guiCommand->slot = 5;
    return NULL;
}

void* handleUndoButton(void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    guiCommand->gameCommand.type = GAME_COMMAND_UNDO;
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}

void* handleBoardEvent(BoardEventArgs *event, void *args) {
    GUICommand *guiCommand = (GUICommand*)args;
    if (event->isRightClick) {
        guiCommand->gameCommand.type = GAME_COMMAND_GET_MOVES;
        memcpy(guiCommand->gameCommand.args, event->move, sizeof(int) * 2);
    } else {
        guiCommand->gameCommand.type = GAME_COMMAND_MOVE;
        memcpy(guiCommand->gameCommand.args, event->move, sizeof(int) * GUI_BOARD_MOVE_ARGS);
    }
    guiCommand->type = GUI_COMMAND_GAME_COMMAND;
    return NULL;
}

Pane* MainPane_Create(SDL_Renderer *renderer) {
    Button *buttons[] = {
        Button_Create(renderer,
                      SRC_BUTTON_NEW_GAME,
                      (SDL_Rect){ .x = 25, .y = 25, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleNewGameButton),
        Button_Create(renderer,
                      SRC_BUTTON_LOAD,
                      (SDL_Rect){ .x = 25, .y = 100, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleLoadPaneButton),
        Button_Create(renderer,
                      SRC_BUTTON_QUIT,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleQuitButton),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       3,
                       NULL, NULL);
}

Pane* SettingsPane_Create(SDL_Renderer *renderer) {
    Button *buttons[] = {
        Button_Create(renderer,
                      SRC_BUTTON_GAME_MODE,
                      (SDL_Rect){ .x = 25, .y = 25, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_DIFFICULTY,
                      (SDL_Rect){ .x = 25, .y = 100, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_USER_COLOR,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_BACK,
                      (SDL_Rect){ .x = 25, .y = 250, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleBackButton),
        Button_Create(renderer,
                      SRC_BUTTON_START,
                      (SDL_Rect){ .x = 25, .y = 325, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleStartButton),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       5,
                       NULL, NULL);
}

Pane* LoadPane_Create(SDL_Renderer *renderer) {
    Button *buttons[] = {
        Button_Create(renderer,
                      SRC_BUTTON_BACK,
                      (SDL_Rect){ .x = 25, .y = 25, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleBackButton),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT1,
                      (SDL_Rect){ .x = 25, .y = 100, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleSlot1Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT2,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleSlot2Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT3,
                      (SDL_Rect){ .x = 25, .y = 250, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleSlot3Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT4,
                      (SDL_Rect){ .x = 25, .y = 325, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleSlot4Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT5,
                      (SDL_Rect){ .x = 25, .y = 400, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleSlot5Button),
        Button_Create(renderer,
                      SRC_BUTTON_LOAD,
                      (SDL_Rect){ .x = 25, .y = 475, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleLoadButton),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       7,
                       NULL, NULL);
}

void onPreRenderUndoButton(Button *button, const void *args) {
    GameManager *manager = (GameManager*)args;
    Button_SetEnabled(button, !ArrayStack_IsEmpty(manager->game->history));
}

Pane* GamePane_Create(SDL_Renderer *renderer) {
    Button *buttons[] = {
        Button_Create(renderer,
                      SRC_BUTTON_RESTART,
                      (SDL_Rect){ .x = 25, .y = 25, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleRestartButton),
        Button_Create(renderer,
                      SRC_BUTTON_SAVE,
                      (SDL_Rect){ .x = 25, .y = 100, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleSaveButton),
        Button_Create(renderer,
                      SRC_BUTTON_LOAD,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleLoadPaneButton),
        Button_Create(renderer,
                      SRC_BUTTON_UNDO,
                      (SDL_Rect){ .x = 25, .y = 250, .w = BUTTON_W, .h = BUTTON_H },
                      onPreRenderUndoButton,
                      handleUndoButton),
        Button_Create(renderer,
                      SRC_BUTTON_MAIN_MENU,
                      (SDL_Rect){ .x = 25, .y = 325, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleBackButton),
        Button_Create(renderer,
                      SRC_BUTTON_QUIT,
                      (SDL_Rect){ .x = 25, .y = 400, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleQuitButton),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       6,
                       NULL, NULL);
}

void pseudoRender(GUIEngine *engine, const GameManager *manager, GameCommand command) {
    SDL_RenderClear(engine->renderer);
    SDL_RenderCopy(engine->renderer, engine->bgTexture, NULL, NULL);
    Pane_Render(engine->pane, manager);
    Board_Render(engine->board, manager, command.type);
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
    engine->slot = 1;
    pseudoRender(engine, NULL, (GameCommand){ .type = GAME_COMMAND_INVALID });
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
        case SDL_QUIT:
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
    Pane_Destroy(engine->pane);
    engine->paneType = type;
    switch (type) {
        case PANE_TYPE_MAIN:
            engine->pane = MainPane_Create(engine->renderer);
            break;
        case PANE_TYPE_SETTINGS:
            engine->pane = SettingsPane_Create(engine->renderer);
            break;
        case PANE_TYPE_LOAD:
            engine->pane = LoadPane_Create(engine->renderer);
            break;
        case PANE_TYPE_GAME:
            engine->pane = GamePane_Create(engine->renderer);
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
        if (engine->event.type != SDL_MOUSEBUTTONUP) continue;
        Pane_HandleEvent(engine->pane, &engine->event, &guiCommand);
        Board_HandleEvent(engine->board, &engine->event, &guiCommand);
        switch (guiCommand.type) {
            case GUI_COMMAND_GAME_COMMAND:
                return guiCommand.gameCommand;
            case GUI_COMMAND_SWITCH_PANE:
                updatePane(engine, guiCommand.paneType);
                pseudoRender(engine, NULL, (GameCommand){ .type = GAME_COMMAND_INVALID });
                break;
            case GUI_COMMAND_BOTH:
                updatePane(engine, guiCommand.paneType);
                pseudoRender(engine, NULL, (GameCommand){ .type = GAME_COMMAND_INVALID });
                return guiCommand.gameCommand;
            default:
                break;
        }
    }
}

void GUIEngine_Render(GUIEngine *engine, const GameManager *manager, GameCommand command) {
    if (!manager || manager->error != GAME_ERROR_NONE) return;
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
        case GAME_COMMAND_RESTART:
        case GAME_COMMAND_QUIT:
        case GAME_COMMAND_INVALID:
        default:
            break;
    }
    pseudoRender(engine, manager, command);
}
