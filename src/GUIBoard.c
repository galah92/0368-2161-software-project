#include <SDL.h>
#include <SDL_video.h>
#include "GUIBoard.h"
#include "GameManager.h"
#include "ChessGame.h"

#define BOARD_S                 760
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


struct Board {
    SDL_Renderer *renderer;
	SDL_Rect location;
	// Button **buttons;
	unsigned int numOfButtons;
	void* (*action)(void*);
};
