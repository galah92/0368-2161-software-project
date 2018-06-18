#include <string.h>
#include <SDL.h>
#include <SDL_video.h>
#include "GUIUtils.h"
#include "ArrayStack.h"


#define SRC_PIECE_NONE          "./gui/pieces/NONE.bmp"
#define SRC_PIECE_WHITE_PAWN    "./gui/pieces/PAWN_W.bmp"
#define SRC_PIECE_WHITE_ROOK    "./gui/pieces/ROOK_W.bmp"
#define SRC_PIECE_WHITE_KNIGHT  "./gui/pieces/KNIGHT_W.bmp"
#define SRC_PIECE_WHITE_BISHOP  "./gui/pieces/BISHOP_W.bmp"
#define SRC_PIECE_WHITE_QUEEN   "./gui/pieces/QUEEN_W.bmp"
#define SRC_PIECE_WHITE_KING    "./gui/pieces/KING_W.bmp"
#define SRC_PIECE_BLACK_PAWN    "./gui/pieces/PAWN_B.bmp"
#define SRC_PIECE_BLACK_ROOK    "./gui/pieces/ROOK_B.bmp"
#define SRC_PIECE_BLACK_KNIGHT  "./gui/pieces/KNIGHT_B.bmp"
#define SRC_PIECE_BLACK_BISHOP  "./gui/pieces/BISHOP_B.bmp"
#define SRC_PIECE_BLACK_QUEEN   "./gui/pieces/QUEEN_B.bmp"
#define SRC_PIECE_BLACK_KING    "./gui/pieces/KING_B.bmp"
#define BOARD_X                 (279)
#define BOARD_Y                 (27)
#define BOARD_S                 (711)
#define TILE_S                  (89)


struct Button {
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Rect location;
	bool isEnabled;
	bool isToggled;
	void (*OnPreRender)(Button *button, const void *args);
	void (*OnClick)(void*);
};

Button* Button_Create(SDL_Renderer *renderer,
					  const char *image,
					  SDL_Rect location,
					  void (*OnPreRender)(Button *button, const void *args),
					  void (*OnClick)(void*)) {
	Button *button = malloc(sizeof(Button));
    if (!button) return Button_Destroy(button);
	SDL_Surface *surface = SDL_LoadBMP(image);
    if (!surface) return Button_Destroy(button);
	button->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!button->texture) return Button_Destroy(button);
	SDL_FreeSurface(surface);
	button->renderer = renderer;
	button->location = location;
	button->isEnabled = true;
	button->isToggled = false;
	button->OnPreRender = OnPreRender;
	button->OnClick = OnClick;
	return button;
}

Button* Button_Destroy(Button* button) {
    if (!button) return NULL;
	if (button->texture) SDL_DestroyTexture(button->texture);
	free(button);
    return NULL;
}

void Button_SetImage(Button *button, const char *image) {
    if (!button) return;
    SDL_Surface *surface = SDL_LoadBMP(image);
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
    button->texture = SDL_CreateTextureFromSurface(button->renderer, surface);
    SDL_FreeSurface(surface);
}

void Button_SetEnabled(Button* button, bool isEnabled) {
	if (!button) return;
	button->isEnabled = isEnabled;
}

void Button_SetToggled(Button* button, bool isToggled) {
	if (!button) return;
	button->isToggled = isToggled;
}

void Button_Render(Button *button, const void *args) {
	if (!button) return;
	if (button->OnPreRender) button->OnPreRender(button, args);
	SDL_RenderCopy(button->renderer, button->texture, NULL, &button->location);
	if (!button->isEnabled) {
		SDL_SetRenderDrawColor(button->renderer, 255, 255, 255, 128);
		SDL_RenderFillRect(button->renderer, &button->location);
	} else if (button->isToggled) {
		SDL_SetRenderDrawColor(button->renderer, 0, 255, 0, 64);
		SDL_RenderFillRect(button->renderer, &button->location);
	}
}

void Button_HandleEvent(Button *button, SDL_Event *event, void *args) {
	if (!button || !button->isEnabled) return;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point = { .x = event->button.x, .y = event->button.y };
		if (SDL_PointInRect(&point, &button->location) && button->OnClick) {
			button->OnClick(args);
		}
	}
}

struct Pane {
	SDL_Renderer *renderer;
	SDL_Rect location;
	Button **buttons;
	unsigned int numOfButtons;
};

Pane* Pane_Create(SDL_Renderer *renderer,
				  SDL_Rect location,
				  Button **buttons,
				  unsigned int numOfButtons) {
	Pane *pane = malloc(sizeof(Pane));
    if (!pane) return Pane_Destroy(pane);
	pane->buttons = malloc(sizeof(Button) * numOfButtons);
	if (!pane->buttons) return Pane_Destroy(pane);
	memcpy(pane->buttons, buttons, sizeof(Button) * numOfButtons);
	pane->renderer = renderer;
	pane->location = location;
	pane->numOfButtons = numOfButtons;
	return pane;
}

Pane* Pane_Destroy(Pane* pane) {
    if (!pane) return NULL;
	for (unsigned int i = 0; i < pane->numOfButtons; i++) {
		Button_Destroy(pane->buttons[i]);
	}
	free(pane);
    return NULL;
}

void Pane_Render(Pane *pane, const void *args) {
	if (!pane) return;
	for (unsigned int i = 0; i < pane->numOfButtons; i++) {
		Button_Render(pane->buttons[i], args);
	}
}

void Pane_HandleEvent(Pane *pane, SDL_Event *event, void *args) {
	if (!pane) return;
	for (unsigned int i = 0; i < pane->numOfButtons; i++) {
		Button_HandleEvent(pane->buttons[i], event, args);
	}
}

struct Board {
    SDL_Renderer *renderer;
    Button *buttons[CHESS_GRID][CHESS_GRID];
    BoardEventArgs eventArgs;
    bool hasFromPos;
    void (*action)(BoardEventArgs*, void*);
};

Board* Board_Create(SDL_Renderer *renderer, void (*action)(BoardEventArgs*, void*)) {
    Board *board = malloc(sizeof(Board));
    if (!board) return Board_Destroy(board);
    SDL_Rect rect = { .w = TILE_S, .h = TILE_S };
    for (int i = 0; i < CHESS_GRID; i++) {
        for (int j = 0; j < CHESS_GRID; j++) {
            rect.x = BOARD_X + i * TILE_S;
            rect.y = BOARD_Y + j * TILE_S;
            board->buttons[i][j] = Button_Create(renderer,
                                               SRC_PIECE_NONE,
                                               rect,
                                               NULL,
                                               NULL);
        }
    }
    board->hasFromPos = false;
    board->renderer = renderer;
    board->action = action;
    return board;
}

Board* Board_Destroy(Board* board) {
    if (!board) return NULL;
    for (int i = 0; i < CHESS_GRID; i++) {
        for (int j = 0; j < CHESS_GRID; j++) {
            Button_Destroy(board->buttons[i][j]);
        }
    }
    free(board);
    return NULL;
}

char* pieceToSrcImage(ChessPiece piece) {
    switch (piece) {
        case CHESS_PIECE_WHITE_PAWN: return SRC_PIECE_WHITE_PAWN;
        case CHESS_PIECE_WHITE_ROOK: return SRC_PIECE_WHITE_ROOK;
        case CHESS_PIECE_WHITE_KNIGHT: return SRC_PIECE_WHITE_KNIGHT;
        case CHESS_PIECE_WHITE_BISHOP: return SRC_PIECE_WHITE_BISHOP;
        case CHESS_PIECE_WHITE_QUEEN: return SRC_PIECE_WHITE_QUEEN;
        case CHESS_PIECE_WHITE_KING: return SRC_PIECE_WHITE_KING;
        case CHESS_PIECE_BLACK_PAWN: return SRC_PIECE_BLACK_PAWN;
        case CHESS_PIECE_BLACK_ROOK: return SRC_PIECE_BLACK_ROOK;
        case CHESS_PIECE_BLACK_KNIGHT: return SRC_PIECE_BLACK_KNIGHT;
        case CHESS_PIECE_BLACK_BISHOP: return SRC_PIECE_BLACK_BISHOP;
        case CHESS_PIECE_BLACK_QUEEN: return SRC_PIECE_BLACK_QUEEN;
        case CHESS_PIECE_BLACK_KING: return SRC_PIECE_BLACK_KING;
        case CHESS_PIECE_NONE:
        default:
            return SRC_PIECE_NONE;
    }
}

void Board_Render(Board *board, const GameManager *manager, GameCommandType commType) {
    if (!board) return;
    if (commType == GAME_COMMAND_GET_MOVES) {        
        ChessPos *pos;
        SDL_Rect rect = { .w=TILE_S, .h=TILE_S };
        while (!ArrayStack_IsEmpty(manager->moves)) {
            pos = ArrayStack_PopLeft(manager->moves);
            rect.x = BOARD_X - 2 + pos->x * TILE_S;
            rect.y = BOARD_Y + 2 + (CHESS_GRID - 1 - pos->y) * TILE_S;
            switch (pos->type) {
                case CHESS_POS_STANDARD:
                    SDL_SetRenderDrawColor(board->renderer, 0, 160, 160, 100);
                    break;
                case CHESS_POS_THREATENED:
                    SDL_SetRenderDrawColor(board->renderer, 160, 0, 0, 100);
                    break;
                case CHESS_POS_CAPTURE:
                    SDL_SetRenderDrawColor(board->renderer, 0, 160, 0, 100);
                    break;
                case CHESS_POS_BOTH:
                    SDL_SetRenderDrawColor(board->renderer, 0, 0, 160, 100);
                    break;
                default:
                    break;
            }
            SDL_RenderFillRect(board->renderer, &rect);
        }
    }
    for (int i = 0; i < CHESS_GRID; i++) {
        for (int j = 0; j < CHESS_GRID; j++) {
            if (manager) {
                Button_SetImage(board->buttons[i][j],
                            pieceToSrcImage(manager->game->board[i][CHESS_GRID - 1 - j]));
            }
            Button_Render(board->buttons[i][j], NULL);
        }
    }
}

bool isInBoardPerimiter(int x, int y) {
    return x > BOARD_X && x < BOARD_X + BOARD_S &&
           y > BOARD_Y && y < BOARD_Y + BOARD_S;
}

void* Board_HandleEvent(Board *board, SDL_Event *event, void *args) {
    if (!board) return NULL;
    if (event->type == SDL_MOUSEBUTTONUP) {
        int x = (event->button.x - BOARD_X) / TILE_S + 1;
        int y = CHESS_GRID - (event->button.y - BOARD_Y) / TILE_S;
        if (event->button.button == SDL_BUTTON_RIGHT) {
            board->eventArgs.isRightClick = true;
            board->eventArgs.move[0] = y;
            board->eventArgs.move[1] = x + 'A' - 1;
            if (board->action) {
                board->action(&board->eventArgs, args);   
            }
        }
        board->eventArgs.isRightClick = false;
        if (!isInBoardPerimiter(event->button.x, event->button.y)) return NULL;
        if (board->hasFromPos) {
            board->eventArgs.move[2] = y;
            board->eventArgs.move[3] = x + 'A' - 1;
            board->hasFromPos = !board->hasFromPos;
            if (board->action) {
                board->action(&board->eventArgs, args);   
            }
        } else {
            board->eventArgs.move[0] = y;
            board->eventArgs.move[1] = x + 'A' - 1;
            board->hasFromPos = !board->hasFromPos;
        }
    }
    return NULL;
}
