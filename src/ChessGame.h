#ifndef CHESS_H_
#define CHESS_H_

#include <stdio.h>
#include "ArrayStack.h"

#define CHESS_GRID 8


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
    CHESS_PLAYER_COLOR_NONE,
} ChessColor;

typedef enum ChessPiece {
    CHESS_PIECE_NONE = '_',
    CHESS_PIECE_WHITE_PAWN = 'm',
    CHESS_PIECE_WHITE_ROOK = 'r',
    CHESS_PIECE_WHITE_KNIGHT = 'n',
    CHESS_PIECE_WHITE_BISHOP = 'b',
    CHESS_PIECE_WHITE_QUEEN = 'q',
    CHESS_PIECE_WHITE_KING = 'k',
    CHESS_PIECE_BLACK_PAWN = 'M',
    CHESS_PIECE_BLACK_ROOK = 'R',
    CHESS_PIECE_BLACK_KNIGHT = 'N',
    CHESS_PIECE_BLACK_BISHOP = 'B',
    CHESS_PIECE_BLACK_QUEEN = 'Q',
    CHESS_PIECE_BLACK_KING = 'K',
} ChessPiece;

typedef struct ChessGame {
    ChessColor turn;
    ChessMode mode;
    ChessDifficulty difficulty;
    ChessColor userColor;
    ChessPiece board[CHESS_GRID][CHESS_GRID];
    ArrayStack *history;
} ChessGame;

typedef enum ChessStatus {
    CHESS_STATUS_RUNNING,
    CHESS_STATUS_CHECK,
    CHESS_STATUS_CHECKMATE,
    CHESS_STATUS_DRAW,
} ChessStatus;

typedef struct ChessPos {
    int x;
    int y;
} ChessPos;

typedef struct ChessMove {
    ChessPos from;
    ChessPos to;
    ChessPiece capturedPiece;
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
 * Calculate a GameStatus of a given ChessGame.
 * @param   game        the instance to calculate a ChessStatus on
 * @param   status      output parameter of the ChessGame's status
 * return   CHESS_INVALID_ARGUMENT if game == NULL
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_GetGameStatus(ChessGame *game, ChessStatus *status);

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
 * Calculate a list of all possible moves for a given ChessPos.
 * The third argument will be redirecred to an ArrayStack* of ChessMove's
 * that should be ArrayList_Destory()'d (caller responsibility).
 * @param   game        the instance to calculate moves on
 * @param   pos         the position to calculate moves from
 * @param   moves       output parameter of the possible moves
 * return   CHESS_INVALID_ARGUMENT if game == NULL
 *          CHESS_INVALID_POSITION is pos is not on board. In that case,
 *              the stack won't be created
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_GetMoves(ChessGame *game, ChessPos pos, ArrayStack **stack);

/**
 * Send a formatted string of a given ChessGame's settings to a given stream.
 * @param   game        the instance to fetch the string from
 * @param   stream      the stream to send the string to
 * return   CHESS_INVALID_ARGUMENT if game == NULL || stream == NULL
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_SettingsToStream(const ChessGame *game, FILE *stream);

/**
 * Send a formatted string of a given ChessGame's board to a given stream.
 * @param   game        the instance to fetch the string from
 * @param   stream      the stream to send the string to
 * return   CHESS_INVALID_ARGUMENT if game == NULL || stream == NULL
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_BoardToStream(const ChessGame *game, FILE *stream);


#endif
