#ifndef GUI_BOARD_H_
#define GUI_BOARD_H_

#define GUI_BOARD_MOVE_ARGS   4


typedef struct BoardEventArgs {
    int move[GUI_BOARD_MOVE_ARGS];
} BoardEventArgs;

typedef struct Board Board;

Board* Board_Create(SDL_Renderer *renderer, void* (*action)(BoardEventArgs*, void*));

Board* Board_Destroy(Board* board);

void Board_Render(Board *board);

void* Board_HandleEvent(Board *board, SDL_Event *event, void *args);


#endif
