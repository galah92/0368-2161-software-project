#ifndef CHESS_H_
#define CHESS_H_

#include "ArrayStack.h"

#define CHESS_GRID 8
#define CHESS_HISTORY_SIZE 3


typedef enum ChessResult {
    CHESS_SUCCESS,
    CHESS_INVALID_ARGUMENT,
    CHESS_INVALID_POSITION,
    CHESS_EMPTY_POSITION,
    CHESS_ILLEGAL_MOVE,
    CHESS_KING_IS_STILL_THREATENED,
    CHESS_KING_WILL_BE_THREATENED,
} ChessResult;

typedef enum ChessMode {
    CHESS_MODE_1_PLAYER = 1,
    CHESS_MODE_2_PLAYER,
} ChessMode;

typedef enum ChessDifficulty {
    CHESS_DIFFICULTY_AMATEUR = 1,
    CHESS_DIFFICULTY_EASY,
    CHESS_DIFFICULTY_MODERATE,
    CHESS_DIFFICULTY_HARD,
    CHESS_DIFFICULTY_EXPERT,
} ChessDifficulty;

typedef enum ChessColor {
    CHESS_PLAYER_COLOR_WHITE,
    CHESS_PLAYER_COLOR_BLACK,
} ChessColor;

typedef struct ChessSettings {
    ChessMode mode;
    ChessDifficulty difficulty;
    ChessColor userColor;
} ChessSettings;

typedef enum ChessStatus {
    CHESS_STATUS_RUNNING,
    CHESS_STATUS_CHECK,
    CHESS_STATUS_CHECKMATE,
    CHESS_STATUS_DRAW,
} ChessStatus;

typedef enum ChessPiece {
    CHESS_PIECE_NONE,
    CHESS_PIECE_PAWN,
    CHESS_PIECE_ROOK,
    CHESS_PIECE_KNIGHT,
    CHESS_PIECE_BISHOP,
    CHESS_PIECE_QUEEN,
    CHESS_PIECE_KING,
} ChessPiece;

typedef struct ChessTile {
    ChessPiece piece;
    ChessColor color;
} ChessTile;

typedef struct ChessGame {
    ChessStatus status;
    ChessSettings settings;
    ChessColor turn;
    ChessTile board[CHESS_GRID][CHESS_GRID];
    ArrayStack *history;
} ChessGame;

typedef struct ChessBoardPos {
    int x;
    int y;
} ChessBoardPos;

typedef struct ChessMove {
    ChessBoardPos from;
    ChessBoardPos to;
    ChessTile capturedPiece;
} ChessMove;

/**
 * Create new ChessGame instance.
 * @return  NULL if malloc failed
 *          ChessGame* instance otherwise
 */
ChessGame* ChessGame_Create();

/**
 * Free all resources for a given ChessGame instance.
 * @param   game        the instance to destroy
 */
void ChessGame_Destroy(ChessGame *game);

/**
 * Apply the default settings to a given game.
 * Defaults are: 1-player game mode, "easy" difficulty, and white player color.
 * @param   game        the instance set defaults to
 */
ChessResult ChessGame_SetDefaultSettings(ChessGame *game);

/**
 * Set a given mode to a given game instance.
 * @param   game        the instance to set mode to
 * @param   mode        the mode to set
 */
ChessResult ChessGame_SetGameMode(ChessGame *game, ChessMode mode);

/**
 * Set a given difficulty to a given game instance.
 * @param   game        the instance to set mode to
 * @param   difficulty  the difficulty to set
 */
ChessResult ChessGame_SetDifficulty(ChessGame *game, ChessDifficulty difficulty);

/**
 * Set a given user color to a given game instance.
 * @param   game        the instance to set mode to
 * @param   userColor   the difficulty to set
 */
ChessResult ChessGame_SetUserColor(ChessGame *game, ChessColor userColor);

ChessResult ChessGame_IsValidMove(ChessGame *game, ChessMove move);

ChessResult ChessGame_DoMove(ChessGame *game, ChessMove move);

ChessResult ChessGame_UndoMove(ChessGame *game);

ChessResult ChessGame_GetMoves(ChessGame *game, ChessBoardPos pos, ChessMove *moves[]);


#endif
