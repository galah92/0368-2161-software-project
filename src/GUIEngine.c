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

#define SRC_BUTTON_AMATEUR      "./gui/colors/amateur.bmp"
#define SRC_BUTTON_EASY         "./gui/colors/easy.bmp"
#define SRC_BUTTON_MODERATE     "./gui/colors/moderate.bmp"
#define SRC_BUTTON_HARD         "./gui/colors/hard.bmp"
#define SRC_BUTTON_EXPERT       "./gui/colors/expert.bmp"

#define SRC_BUTTON_1_PLAYER     "./gui/colors/1playermode.bmp"
#define SRC_BUTTON_2_PLAYER     "./gui/colors/2playermode.bmp"


struct GUIEngine {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *bgTexture;
    SDL_Event event;
    GamePaneType paneType;
    Pane *pane;
    Board *board;
};

void* handleNewGameButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_PANE;
    command->args[0] = GAME_PANE_TYPE_SETTINGS;
    return NULL;
}

void* handleLoadPaneButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_PANE;
    command->args[0] = GAME_PANE_TYPE_LOAD;
    return NULL;
}

void* handleQuitButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_QUIT;
    return NULL;
}

void* handleBackButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_BACK_PANE;
    return NULL;
}

void* handleStartButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_START;
    return NULL;
}

void* handleRestartButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_RESTART;
    return NULL;
}

void* handleLoadButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_LOAD_AND_START;
    return NULL;
}

void* handleSaveButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SAVE_FROM_SLOT;
    return NULL;
}

// void onPreRenderSlot1Button(Button *button, const void *args) {
//     Button_SetToggled(button, *(unsigned int*)args == 1);
// }
// void onPreRenderSlot2Button(Button *button, const void *args) {
//     Button_SetToggled(button, *(unsigned int*)args == 2);
// }
// void onPreRenderSlot3Button(Button *button, const void *args) {
//     Button_SetToggled(button, *(unsigned int*)args == 3);
// }
// void onPreRenderSlot4Button(Button *button, const void *args) {
//     Button_SetToggled(button, *(unsigned int*)args == 4);
// }
// void onPreRenderSlot5Button(Button *button, const void *args) {
//     Button_SetToggled(button, *(unsigned int*)args == 5);
// }
void* handleSlot1Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_SLOT;
    command->args[0] = 1;
    return NULL;
}
void* handleSlot2Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_SLOT;
    command->args[0] = 2;
    return NULL;
}
void* handleSlot3Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_SLOT;
    command->args[0] = 3;
    return NULL;
}
void* handleSlot4Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_SLOT;
    command->args[0] = 4;
    return NULL;
}
void* handleSlot5Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_SLOT;
    command->args[0] = 5;
    return NULL;
}

// void onPreRenderDifficulty1Button(Button *button, const void *args) {
//     GameManager *manager = (GameManager*)args;
//     Button_SetToggled(button, manager->game->difficulty == 1);
// }
// void onPreRenderDifficulty2Button(Button *button, const void *args) {
//     GameManager *manager = (GameManager*)args;
//     Button_SetToggled(button, manager->game->difficulty == 2);
// }
// void onPreRenderDifficulty3Button(Button *button, const void *args) {
//     GameManager *manager = (GameManager*)args;
//     Button_SetToggled(button, manager->game->difficulty == 3);
// }
// void onPreRenderDifficulty4Button(Button *button, const void *args) {
//     GameManager *manager = (GameManager*)args;
//     Button_SetToggled(button, manager->game->difficulty == 4);
// }
// void onPreRenderDifficulty5Button(Button *button, const void *args) {
//     GameManager *manager = (GameManager*)args;
//     Button_SetToggled(button, manager->game->difficulty == 5);
// }
void* handleDifficulty1Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_DIFFICULTY;
    command->args[0] = 1;
    return NULL;
}
void* handleDifficulty2Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_DIFFICULTY;
    command->args[0] = 2;
    return NULL;
}
void* handleDifficulty3Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_DIFFICULTY;
    command->args[0] = 3;
    return NULL;
}
void* handleDifficulty4Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_DIFFICULTY;
    command->args[0] = 4;
    return NULL;
}
void* handleDifficulty5Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_DIFFICULTY;
    command->args[0] = 5;
    return NULL;
}

void* handleUndoButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_UNDO;
    return NULL;
}

void* handleBoardEvent(BoardEventArgs *event, void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = event->isRightClick ? GAME_COMMAND_GET_MOVES : GAME_COMMAND_MOVE;
    memcpy(command->args, event->move, sizeof(int) * GUI_BOARD_MOVE_ARGS);
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
                      SRC_BUTTON_1_PLAYER,
                      (SDL_Rect){ .x = 25, .y = 100, .w = 90, .h = BUTTON_H },
                      NULL,
                      handleNewGameButton),
        Button_Create(renderer,
                      SRC_BUTTON_2_PLAYER,
                      (SDL_Rect){ .x = 135, .y = 100, .w = 90, .h = BUTTON_H },
                      NULL,
                      handleNewGameButton),
        Button_Create(renderer,
                      SRC_BUTTON_DIFFICULTY,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_AMATEUR,
                      (SDL_Rect){ .x = 25, .y = 250, .w = 90, .h = BUTTON_H },
                      NULL,
                      handleDifficulty1Button),
        Button_Create(renderer,
                      SRC_BUTTON_EASY,
                      (SDL_Rect){ .x = 135, .y = 250, .w = 90, .h = BUTTON_H },
                      NULL,
                      handleDifficulty2Button),
        Button_Create(renderer,
                      SRC_BUTTON_MODERATE,
                      (SDL_Rect){ .x = 25, .y = 325, .w = 90, .h = BUTTON_H },
                      NULL,
                      handleDifficulty3Button),
        Button_Create(renderer,
                      SRC_BUTTON_HARD,
                      (SDL_Rect){ .x = 135, .y = 325, .w = 90, .h = BUTTON_H },
                      NULL,
                      handleDifficulty4Button),
        Button_Create(renderer,
                      SRC_BUTTON_EXPERT,
                      (SDL_Rect){ .x = 80, .y = 400, .w = 90, .h = BUTTON_H },
                      NULL,
                      handleDifficulty5Button),              
        Button_Create(renderer,
                      SRC_BUTTON_USER_COLOR,
                      (SDL_Rect){ .x = 25, .y = 475, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_BACK,
                      (SDL_Rect){ .x = 25, .y = 550, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleBackButton),
        Button_Create(renderer,
                      SRC_BUTTON_START,
                      (SDL_Rect){ .x = 25, .y = 625, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleStartButton),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       12,
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
                      (SDL_Rect){ .x = 25, .y = 100, .w = 90, .h = BUTTON_H },
                      NULL,
                      handleSlot1Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT2,
                      (SDL_Rect){ .x = 135, .y = 100, .w = 90, .h = BUTTON_H },
                      NULL,
                      handleSlot2Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT3,
                      (SDL_Rect){ .x = 25, .y = 175, .w = 90, .h = BUTTON_H },
                      NULL,
                      handleSlot3Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT4,
                      (SDL_Rect){ .x = 135, .y = 175, .w = 90, .h = BUTTON_H },
                      NULL,
                      handleSlot4Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT5,
                      (SDL_Rect){ .x = 80, .y = 250, .w = 90, .h = BUTTON_H },
                      NULL,
                      handleSlot5Button),
        Button_Create(renderer,
                      SRC_BUTTON_LOAD,
                      (SDL_Rect){ .x = 25, .y = 325, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleLoadButton),
        Button_Create(renderer,
                      SRC_BUTTON_SAVE,
                      (SDL_Rect){ .x = 25, .y = 400, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleSaveButton),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       8,
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
                      handleLoadPaneButton),
        Button_Create(renderer,
                      SRC_BUTTON_LOAD,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      handleLoadPaneButton),
        Button_Create(renderer,
                      SRC_BUTTON_UNDO,
                      (SDL_Rect){ .x = 25, .y = 250, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
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
    engine->pane = MainPane_Create(engine->renderer);
    engine->board = Board_Create(engine->renderer, handleBoardEvent);
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

void updatePane(GUIEngine *engine, GamePaneType type) {
    Pane_Destroy(engine->pane);
    engine->paneType = type;
    switch (type) {
        case GAME_PANE_TYPE_MAIN:
            engine->pane = MainPane_Create(engine->renderer);
            break;
        case GAME_PANE_TYPE_SETTINGS:
            engine->pane = SettingsPane_Create(engine->renderer);
            break;
        case GAME_PANE_TYPE_LOAD:
            engine->pane = LoadPane_Create(engine->renderer);
            break;
        case GAME_PANE_TYPE_GAME:
            engine->pane = GamePane_Create(engine->renderer);
            break;
        default:
            break;
    }
}

GameCommand GUIEngine_ProcessInput(GUIEngine *engine) {
    GameCommand command = { .type = GAME_COMMAND_INVALID };
    if (!engine) return command;
    while (command.type == GAME_COMMAND_INVALID) {
        SDL_WaitEvent(&engine->event);
        if (isExitButtonEvent(&engine->event)) {
            command.type = GAME_COMMAND_QUIT;
            return command;
        }
        if (engine->event.type != SDL_MOUSEBUTTONUP) continue;
        Pane_HandleEvent(engine->pane, &engine->event, &command);
        Board_HandleEvent(engine->board, &engine->event, &command);
    }
    return command;
    // GUICommand guiCommand = { .type = GUI_COMMAND_NONE };
    // guiCommand.gameCommand = (GameCommand){ .type = GAME_COMMAND_INVALID };
    // if (!engine) return guiCommand.gameCommand;
    // while (true) {
    //     SDL_WaitEvent(&engine->event);
    //     if (isExitButtonEvent(&engine->event)) {
    //         guiCommand.gameCommand.type = GAME_COMMAND_QUIT;
    //         return guiCommand.gameCommand;
    //     }
    //     if (engine->event.type != SDL_MOUSEBUTTONUP) continue;
    //     guiCommand.slot = engine->slot;
    //     Pane_HandleEvent(engine->pane, &engine->event, &guiCommand);
    //     Board_HandleEvent(engine->board, &engine->event, &guiCommand);
    //     switch (guiCommand.type) {
    //         case GUI_COMMAND_GAME_COMMAND:
    //             return guiCommand.gameCommand;
    //         case GUI_COMMAND_SWITCH_PANE:
    //             updatePane(engine, guiCommand.paneType);
    //             pseudoRender(engine, NULL, (GameCommand){ .type = GAME_COMMAND_INVALID });
    //             break;
    //         case GUI_COMMAND_SET_SLOT:
    //             engine->slot = guiCommand.slot;
    //             pseudoRender(engine, NULL, (GameCommand){ .type = GAME_COMMAND_INVALID });
    //             break;
    //         case GUI_COMMAND_BOTH:
    //             updatePane(engine, guiCommand.paneType);
    //             pseudoRender(engine, NULL, (GameCommand){ .type = GAME_COMMAND_INVALID });
    //             return guiCommand.gameCommand;
    //         default:
    //             break;
    //     }
    // }
}

void GUIEngine_Render(GUIEngine *engine, const GameManager *manager, GameCommand command) {
    if (!manager || manager->error != GAME_ERROR_NONE) return;
    printf("%d, %d\n", command.type == GAME_COMMAND_SET_PANE, manager->paneType == GAME_PANE_TYPE_SETTINGS);
    switch (command.type) {
        case GAME_COMMAND_LOAD_AND_START:
        case GAME_COMMAND_START:
        case GAME_COMMAND_SET_PANE:
        case GAME_COMMAND_BACK_PANE:
            updatePane(engine, manager->paneType);
            break;
        case GAME_COMMAND_DEFAULT_SETTINGS:
        case GAME_COMMAND_PRINT_SETTINGS:
        case GAME_COMMAND_GAME_MODE:
        case GAME_COMMAND_DIFFICULTY:
        case GAME_COMMAND_USER_COLOR:
        case GAME_COMMAND_MOVE:
        case GAME_COMMAND_GET_MOVES:
        case GAME_COMMAND_SAVE:
        case GAME_COMMAND_UNDO:
        case GAME_COMMAND_RESET:
        case GAME_COMMAND_RESTART:
        case GAME_COMMAND_QUIT:
        case GAME_COMMAND_INVALID:        
        case GAME_COMMAND_SET_SLOT:
        default:
            break;
    }
    pseudoRender(engine, manager, command);
}
