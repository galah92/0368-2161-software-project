#ifndef GUI_BOARD_H_
#define GUI_BOARD_H_

#include <stdbool.h>
#include "GameManager.h"
#include "ChessGame.h"

#define GUI_BOARD_MOVE_ARGS   4


typedef struct BoardEventArgs {
    int move[GUI_BOARD_MOVE_ARGS];
    bool isRightClick;
} BoardEventArgs;

typedef struct Board Board;

Board* Board_Create(SDL_Renderer *renderer, void* (*action)(BoardEventArgs*, void*));

Board* Board_Destroy(Board* board);

void Board_Render(Board *board, const GameManager *manager, GameCommandType commType);

void* Board_HandleEvent(Board *board, SDL_Event *event, void *args);


#endif
