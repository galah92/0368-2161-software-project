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
    CHESS_EMPTY_HISTORY,
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
 * return   CHESS_INVALID_ARGUMENT if game == NULL
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_SetDefaultSettings(ChessGame *game);

/**
 * Set a given mode to a given game instance.
 * @param   game        the instance to set mode to
 * @param   mode        the mode to set
 * return   CHESS_INVALID_ARGUMENT if game == NULL or mode is not of ChessMode
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_SetGameMode(ChessGame *game, ChessMode mode);

/**
 * Set a given difficulty to a given game instance.
 * @param   game        the instance to set mode to
 * @param   difficulty  the difficulty to set
 * return   CHESS_INVALID_ARGUMENT if game == NULL or
 *              difficulty is not of ChessDifficulty
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_SetDifficulty(ChessGame *game, ChessDifficulty difficulty);

/**
 * Set a given user color to a given game instance.
 * @param   game        the instance to set mode to
 * @param   userColor   the difficulty to set
 * return   CHESS_INVALID_ARGUMENT if game == NULL or
 *              userColor is not of ChessColor
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_SetUserColor(ChessGame *game, ChessColor userColor);

/**
 * Check if a given ChessMove is a valid next move for a given ChessGame.
 * @param   game        the instance to set mode to
 * @param   move        the move to check it's validity
 * return   CHESS_INVALID_ARGUMENT if game == NULL
 *          CHESS_INVALID_POSITION if move locations are not on board
 *          CHESS_EMPTY_POSITION if the source position doesn't contain
 *              the player's piece
 *          CHESS_ILLEGAL_MOVE if the piece can't move in that way,
 *              or if the destination position contains a player's piece
 *          CHESS_KING_IS_STILL_THREATENED if the status is CHECK and
 *              the move won't change that status
 *          CHESS_KING_WILL_BE_THREATENED if the move will expose the
 *              player to CHECK
 *          CHESS_SUCCESS otherwise (there's no other choise left!)
 */
ChessResult ChessGame_IsValidMove(ChessGame *game, ChessMove move);

/**
 * Apply a given ChessMove on a given ChessGame after checking it's validity.
 * @param   game        the instance to apply the move on
 * @param   move        the move to apply
 * return   the ChessGame_IsValidMove() result if it failed
 *          CHESS_SUCCESS otherwise (after applying the move)
 */
ChessResult ChessGame_DoMove(ChessGame *game, ChessMove move);

/**
 * Undo the last move of a given ChessGame.
 * @param   game        the instance to undo a move on
 * return   CHESS_EMPTY_HISTORY if there are no moves to undo
 *          CHESS_SUCCESS otherwise (after undo a move)
 */
ChessResult ChessGame_UndoMove(ChessGame *game);

/**
 * Calculate a list of all possible moves for a given ChessBoardPos.
 * The third argument must be an initialized ArrayStack of ChessMove's.
 * It will be populated with the calculated moves.
 * @param   game        the instance to calculate moves on
 * return   CHESS_SUCCESS always
 */
ChessResult ChessGame_GetMoves(ChessGame *game, ChessBoardPos pos, ArrayStack *stack);


#endif
