#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
#include <SDL_video.h>
#include "GUIEngine.h"
#include "GUIUtils.h"
#include "GUIBoard.h"
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

void onClickNewGameButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_PANE;
    command->args[0] = GAME_PANE_TYPE_SETTINGS;
}

void onClickLoadPaneButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_PANE;
    command->args[0] = GAME_PANE_TYPE_LOAD;
}

void onClickQuitButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_QUIT;
}

void onClickBackButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_BACK_PANE;
}

void onClickStartButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_START;
}

void onClickRestartButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_RESTART;
}

void onClickLoadButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_LOAD_AND_START;
}

void onClickSaveButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SAVE_FROM_SLOT;
}

void onPreRenderSlot1Button(Button *button, const void *args) {
    GameManager *manager = (GameManager*)args;
    Button_SetToggled(button, manager->slot == 1);
}

void onPreRenderSlot2Button(Button *button, const void *args) {
    GameManager *manager = (GameManager*)args;
    Button_SetToggled(button, manager->slot == 2);
}

void onPreRenderSlot3Button(Button *button, const void *args) {
    GameManager *manager = (GameManager*)args;
    Button_SetToggled(button, manager->slot == 3);
}

void onPreRenderSlot4Button(Button *button, const void *args) {
    GameManager *manager = (GameManager*)args;
    Button_SetToggled(button, manager->slot == 4);
}

void onPreRenderSlot5Button(Button *button, const void *args) {
    GameManager *manager = (GameManager*)args;
    Button_SetToggled(button, manager->slot == 5);
}

void onClickSlot1Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_SLOT;
    command->args[0] = 1;
}
void onClickSlot2Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_SLOT;
    command->args[0] = 2;
}
void onClickSlot3Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_SLOT;
    command->args[0] = 3;
}
void onClickSlot4Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_SLOT;
    command->args[0] = 4;
}
void onClickSlot5Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_SET_SLOT;
    command->args[0] = 5;
}

void onPreRenderGameMode1Button(Button *button, const void *args) {
    GameManager *manager = (GameManager*)args;
    Button_SetToggled(button, manager->game->mode == CHESS_MODE_1_PLAYER);
}
void onPreRenderGameMode2Button(Button *button, const void *args) {
    GameManager *manager = (GameManager*)args;
    Button_SetToggled(button, manager->game->mode == CHESS_MODE_2_PLAYER);
}
void onClickGameMode1Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_GAME_MODE;
    command->args[0] = CHESS_MODE_1_PLAYER;
}
void onClickGameMode2Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_GAME_MODE;
    command->args[0] = CHESS_MODE_2_PLAYER;
}

void onPreRenderDifficulty1Button(Button *button, const void *args) {
    GameManager *manager = (GameManager*)args;
    Button_SetToggled(button, manager->game->difficulty == CHESS_DIFFICULTY_AMATEUR);
}
void onPreRenderDifficulty2Button(Button *button, const void *args) {
    GameManager *manager = (GameManager*)args;
    Button_SetToggled(button, manager->game->difficulty == CHESS_DIFFICULTY_EASY);
}
void onPreRenderDifficulty3Button(Button *button, const void *args) {
    GameManager *manager = (GameManager*)args;
    Button_SetToggled(button, manager->game->difficulty == CHESS_DIFFICULTY_MODERATE);
}
void onPreRenderDifficulty4Button(Button *button, const void *args) {
    GameManager *manager = (GameManager*)args;
    Button_SetToggled(button, manager->game->difficulty == CHESS_DIFFICULTY_HARD);
}
void onPreRenderDifficulty5Button(Button *button, const void *args) {
    GameManager *manager = (GameManager*)args;
    Button_SetToggled(button, manager->game->difficulty == CHESS_DIFFICULTY_EXPERT);
}

void onClickDifficulty1Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_DIFFICULTY;
    command->args[0] = CHESS_DIFFICULTY_AMATEUR;
}
void onClickDifficulty2Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_DIFFICULTY;
    command->args[0] = CHESS_DIFFICULTY_EASY;
}
void onClickDifficulty3Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_DIFFICULTY;
    command->args[0] = CHESS_DIFFICULTY_MODERATE;
}
void onClickDifficulty4Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_DIFFICULTY;
    command->args[0] = CHESS_DIFFICULTY_HARD;
}
void onClickDifficulty5Button(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_DIFFICULTY;
    command->args[0] = CHESS_DIFFICULTY_EXPERT;
}

void onClickUndoButton(void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = GAME_COMMAND_UNDO;
}

void onClickBoardEvent(BoardEventArgs *event, void *args) {
    GameCommand *command = (GameCommand*)args;
    command->type = event->isRightClick ? GAME_COMMAND_GET_MOVES : GAME_COMMAND_MOVE;
    memcpy(command->args, event->move, sizeof(int) * GUI_BOARD_MOVE_ARGS);
}

Pane* MainPane_Create(SDL_Renderer *renderer) {
    Button *buttons[] = {
        Button_Create(renderer,
                      SRC_BUTTON_NEW_GAME,
                      (SDL_Rect){ .x = 25, .y = 25, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      onClickNewGameButton),
        Button_Create(renderer,
                      SRC_BUTTON_LOAD,
                      (SDL_Rect){ .x = 25, .y = 100, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      onClickLoadPaneButton),
        Button_Create(renderer,
                      SRC_BUTTON_QUIT,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      onClickQuitButton),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       3);
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
                      onPreRenderGameMode1Button,
                      onClickGameMode1Button),
        Button_Create(renderer,
                      SRC_BUTTON_2_PLAYER,
                      (SDL_Rect){ .x = 135, .y = 100, .w = 90, .h = BUTTON_H },
                      onPreRenderGameMode2Button,
                      onClickGameMode2Button),
        Button_Create(renderer,
                      SRC_BUTTON_DIFFICULTY,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_AMATEUR,
                      (SDL_Rect){ .x = 25, .y = 250, .w = 90, .h = BUTTON_H },
                      onPreRenderDifficulty1Button,
                      onClickDifficulty1Button),
        Button_Create(renderer,
                      SRC_BUTTON_EASY,
                      (SDL_Rect){ .x = 135, .y = 250, .w = 90, .h = BUTTON_H },
                      onPreRenderDifficulty2Button,
                      onClickDifficulty2Button),
        Button_Create(renderer,
                      SRC_BUTTON_MODERATE,
                      (SDL_Rect){ .x = 25, .y = 325, .w = 90, .h = BUTTON_H },
                      onPreRenderDifficulty3Button,
                      onClickDifficulty3Button),
        Button_Create(renderer,
                      SRC_BUTTON_HARD,
                      (SDL_Rect){ .x = 135, .y = 325, .w = 90, .h = BUTTON_H },
                      onPreRenderDifficulty4Button,
                      onClickDifficulty4Button),
        Button_Create(renderer,
                      SRC_BUTTON_EXPERT,
                      (SDL_Rect){ .x = 80, .y = 400, .w = 90, .h = BUTTON_H },
                      onPreRenderDifficulty5Button,
                      onClickDifficulty5Button),              
        Button_Create(renderer,
                      SRC_BUTTON_USER_COLOR,
                      (SDL_Rect){ .x = 25, .y = 475, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      NULL),
        Button_Create(renderer,
                      SRC_BUTTON_BACK,
                      (SDL_Rect){ .x = 25, .y = 550, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      onClickBackButton),
        Button_Create(renderer,
                      SRC_BUTTON_START,
                      (SDL_Rect){ .x = 25, .y = 625, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      onClickStartButton),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       12);
}

Pane* LoadPane_Create(SDL_Renderer *renderer) {
    Button *buttons[] = {
        Button_Create(renderer,
                      SRC_BUTTON_BACK,
                      (SDL_Rect){ .x = 25, .y = 25, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      onClickBackButton),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT1,
                      (SDL_Rect){ .x = 25, .y = 100, .w = 90, .h = BUTTON_H },
                      onPreRenderSlot1Button,
                      onClickSlot1Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT2,
                      (SDL_Rect){ .x = 135, .y = 100, .w = 90, .h = BUTTON_H },
                      onPreRenderSlot2Button,
                      onClickSlot2Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT3,
                      (SDL_Rect){ .x = 25, .y = 175, .w = 90, .h = BUTTON_H },
                      onPreRenderSlot3Button,
                      onClickSlot3Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT4,
                      (SDL_Rect){ .x = 135, .y = 175, .w = 90, .h = BUTTON_H },
                      onPreRenderSlot4Button,
                      onClickSlot4Button),
        Button_Create(renderer,
                      SRC_BUTTON_SLOT5,
                      (SDL_Rect){ .x = 80, .y = 250, .w = 90, .h = BUTTON_H },
                      onPreRenderSlot5Button,
                      onClickSlot5Button),
        Button_Create(renderer,
                      SRC_BUTTON_LOAD,
                      (SDL_Rect){ .x = 25, .y = 325, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      onClickLoadButton),
        Button_Create(renderer,
                      SRC_BUTTON_SAVE,
                      (SDL_Rect){ .x = 25, .y = 400, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      onClickSaveButton),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       8);
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
                      onClickRestartButton),
        Button_Create(renderer,
                      SRC_BUTTON_SAVE,
                      (SDL_Rect){ .x = 25, .y = 100, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      onClickLoadPaneButton),
        Button_Create(renderer,
                      SRC_BUTTON_LOAD,
                      (SDL_Rect){ .x = 25, .y = 175, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      onClickLoadPaneButton),
        Button_Create(renderer,
                      SRC_BUTTON_UNDO,
                      (SDL_Rect){ .x = 25, .y = 250, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      onClickUndoButton),
        Button_Create(renderer,
                      SRC_BUTTON_MAIN_MENU,
                      (SDL_Rect){ .x = 25, .y = 325, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      onClickBackButton),
        Button_Create(renderer,
                      SRC_BUTTON_QUIT,
                      (SDL_Rect){ .x = 25, .y = 400, .w = BUTTON_W, .h = BUTTON_H },
                      NULL,
                      onClickQuitButton),
    };
    return Pane_Create(renderer,
                       (SDL_Rect){ .x = 0, .y = 0, .w = WINDOW_W, .h = WINDOW_H },
                       buttons,
                       6);
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
    engine->board = Board_Create(engine->renderer, onClickBoardEvent);
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
}

void GUIEngine_Render(GUIEngine *engine, const GameManager *manager, GameCommand command) {
    if (!manager || manager->error != GAME_ERROR_NONE) return;
    switch (command.type) {
        case GAME_COMMAND_LOAD_AND_START:
        case GAME_COMMAND_START:
        case GAME_COMMAND_SET_PANE:
        case GAME_COMMAND_BACK_PANE:
            updatePane(engine, manager->paneType);
            break;
        default:
            break;
    }
    SDL_RenderClear(engine->renderer);
    SDL_RenderCopy(engine->renderer, engine->bgTexture, NULL, NULL);
    Pane_Render(engine->pane, manager);
    Board_Render(engine->board, manager, command.type);
    SDL_RenderPresent(engine->renderer);
}
