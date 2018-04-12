#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>
#include "GUIBoard.h"
#include "GUIUtils.h"

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


// Tile widget

typedef struct Tile Tile;

struct Tile {
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect location;
    void* (*action)();
};

Tile* Tile_Create(SDL_Renderer *renderer,
                  const char *image,
                  SDL_Rect location,
                  void* (*action)());

Tile* Tile_Destroy(Tile* tile);

void Tile_SetImage(Tile *tile, const char *image);

void Tile_Render(Tile *tile);

void* Tile_HandleEvent(Tile *tile, SDL_Event *event);

Tile* Tile_Create(SDL_Renderer *renderer,
                  const char *image,
                  SDL_Rect location,
                  void* (*action)()) {
    Tile *tile = malloc(sizeof(Tile));
    if (!tile) return Tile_Destroy(tile);
    SDL_Surface *surface = SDL_LoadBMP(image);
    if (!surface) return Tile_Destroy(tile);
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
    tile->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!tile->texture) return Tile_Destroy(tile);
    SDL_FreeSurface(surface);
    tile->renderer = renderer;
    tile->location = location;
    tile->action = action;
    return tile;
}

Tile* Tile_Destroy(Tile* tile) {
    if (!tile) return NULL;
    if (tile->texture) SDL_DestroyTexture(tile->texture);
    free(tile);
    return NULL;
}

void Tile_SetImage(Tile *tile, const char *image) {
    if (!tile) return;
    SDL_Surface *surface = SDL_LoadBMP(image);
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));
    tile->texture = SDL_CreateTextureFromSurface(tile->renderer, surface);
    SDL_FreeSurface(surface);
}

void Tile_Render(Tile *tile) {
    if (!tile) return;
    SDL_RenderCopy(tile->renderer, tile->texture, NULL, &tile->location);
}

void* Tile_HandleEvent(Tile *tile, SDL_Event *event) {
    if (!tile) return NULL;
    if (event->type == SDL_MOUSEBUTTONUP) {
        SDL_Point point = { .x = event->button.x, .y = event->button.y };
        if (SDL_PointInRect(&point, &tile->location) && tile->action) {
            tile->location.x = point.x;
            tile->location.y = point.y;
            return tile->action();
        }
    }
    return NULL;
}

struct Board {
    SDL_Renderer *renderer;
    Tile *tiles[CHESS_GRID][CHESS_GRID];
    BoardEventArgs eventArgs;
    bool hasFromPos;
    void* (*action)(BoardEventArgs*, void*);
};

Board* Board_Create(SDL_Renderer *renderer, void* (*action)(BoardEventArgs*, void*)) {
    Board *board = malloc(sizeof(Board));
    if (!board) return Board_Destroy(board);
    SDL_Rect rect = { .w = TILE_S, .h = TILE_S };
    for (int i = 0; i < CHESS_GRID; i++) {
        for (int j = 0; j < CHESS_GRID; j++) {
            rect.x = BOARD_X + i * TILE_S;
            rect.y = BOARD_Y + j * TILE_S;
            board->tiles[i][j] = Tile_Create(renderer,
                                             SRC_PIECE_NONE,
                                             rect,
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
            Tile_Destroy(board->tiles[i][j]);
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

void Board_Render(Board *board, const ChessGame *game) {
    if (!board) return;
    for (int i = 0; i < CHESS_GRID; i++) {
        for (int j = CHESS_GRID - 1; j >= 0; j--) {
            if (game) {
                Tile_SetImage(board->tiles[i][j], pieceToSrcImage(game->board[i][CHESS_GRID - 1 - j]));
            }
            Tile_Render(board->tiles[i][j]);
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
        }
        board->eventArgs.isRightClick = false;
        if (!isInBoardPerimiter(event->button.x, event->button.y)) return NULL;
        if (board->hasFromPos) {
            board->eventArgs.move[2] = y;
            board->eventArgs.move[3] = x + 'A' - 1;
            board->hasFromPos = !board->hasFromPos;
            if (board->action) return board->action(&board->eventArgs, args);
        } else {
            board->eventArgs.move[0] = y;
            board->eventArgs.move[1] = x + 'A' - 1;
            board->hasFromPos = !board->hasFromPos;
        }
    }
    return NULL;
}
