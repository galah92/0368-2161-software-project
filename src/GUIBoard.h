#ifndef GUI_BOARD_H_
#define GUI_BOARD_H_


typedef struct Board Board;

Board* Board_Create(SDL_Renderer *renderer,
                      const char *image,
                      SDL_Rect location,
                      void* (*action)(void*));

Board* Board_Destroy(Board* board);

void Board_Render(Board *board);

void* Board_HandleEvent(Board *board, SDL_Event *event, void *args);


#endif
