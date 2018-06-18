#ifndef CHESS_GAME_H_
#define CHESS_GAME_H_

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
    CHESS_PLAYER_COLOR_BLACK,
    CHESS_PLAYER_COLOR_WHITE,
    CHESS_PLAYER_COLOR_NONE,
} ChessColor;

typedef enum ChessPiece {
    CHESS_PIECE_NONE            = '_',
    CHESS_PIECE_WHITE_PAWN      = 'm',
    CHESS_PIECE_WHITE_ROOK      = 'r',
    CHESS_PIECE_WHITE_KNIGHT    = 'n',
    CHESS_PIECE_WHITE_BISHOP    = 'b',
    CHESS_PIECE_WHITE_QUEEN     = 'q',
    CHESS_PIECE_WHITE_KING      = 'k',
    CHESS_PIECE_BLACK_PAWN      = 'M',
    CHESS_PIECE_BLACK_ROOK      = 'R',
    CHESS_PIECE_BLACK_KNIGHT    = 'N',
    CHESS_PIECE_BLACK_BISHOP    = 'B',
    CHESS_PIECE_BLACK_QUEEN     = 'Q',
    CHESS_PIECE_BLACK_KING      = 'K',
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

typedef enum ChessPosType {
    CHESS_POS_STANDARD,
    CHESS_POS_THREATENED,
    CHESS_POS_CAPTURE,
    CHESS_POS_BOTH,
} ChessPosType;

typedef struct ChessPos {
    int x;
    int y;
    ChessPosType type;
} ChessPos;

typedef struct ChessMove {
    ChessPos from;
    ChessPos to;
    ChessPiece capturedPiece;
    ChessColor player;
} ChessMove;

/**
 * Create new ChessGame instance.
 * @return  NULL if malloc failed
 *          ChessGame* instance otherwise
 */
ChessGame* ChessGame_Create();

/**
 * Create a copy of a given ChessGame instance.
 * @param   game        the instance to copy
 * @return  NULL if malloc failed
 *          ChessGame* instance otherwise
 */
ChessGame* ChessGame_Copy(const ChessGame *game);

/**
 * Free all resources for a given ChessGame instance.
 * @param   game        the instance to destroy
 * @return  NULL
 */
ChessGame* ChessGame_Destroy(ChessGame *game);

/**
 * Reset a given ChessGame turn, board & history.
 * Keep the other settings as they are.
 * @param   game        the instance to reset
 * @return  CHESS_INVALID_ARGUMENT if game == NULL
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_ResetGame(ChessGame *game);

/**
 * Apply the default settings to a given game.
 * Defaults are: 1-player game mode, "easy" difficulty, and white player color.
 * @param   game        the instance set defaults to
 * @return  CHESS_INVALID_ARGUMENT if game == NULL
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_SetDefaultSettings(ChessGame *game);

/**
 * Set a given mode to a given game instance.
 * @param   game        the instance to set mode to
 * @param   mode        the mode to set
 * @return  CHESS_INVALID_ARGUMENT if game == NULL or mode is not of ChessMode
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_SetGameMode(ChessGame *game, ChessMode mode);

/**
 * Set a given difficulty to a given game instance.
 * @param   game        the instance to set mode to
 * @param   difficulty  the difficulty to set
 * @return  CHESS_INVALID_ARGUMENT if game == NULL or
 *              difficulty is not of ChessDifficulty
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_SetDifficulty(ChessGame *game, ChessDifficulty difficulty);

/**
 * Set a given user color to a given game instance.
 * @param   game        the instance to set mode to
 * @param   userColor   the difficulty to set
 * @return  CHESS_INVALID_ARGUMENT if game == NULL or
 *              userColor is not of ChessColor
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_SetUserColor(ChessGame *game, ChessColor userColor);

/**
 * Init a given ChessGame instane's board according to chess rules.
 * White is at the "bottom" of the board - lower indexes.
 * @param   game        the instance init board to
 * @return  CHESS_INVALID_ARGUMENT if game == NULL
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_InitBoard(ChessGame *game);

/**
 * Calculate a GameStatus of a given ChessGame.
 * @param   game        the instance to calculate a ChessStatus on
 * @param   status      output parameter of the ChessGame's status
 * @return  CHESS_INVALID_ARGUMENT if game == NULL
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_GetGameStatus(ChessGame *game, ChessStatus *status);

/**
 * Apply a given ChessMove on a given ChessGame after checking it's validity.
 * @param   game        the instance to apply the move on
 * @param   move        the move to apply
 * @return  CHESS_INVALID_ARGUMENT if game == NULL
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
ChessResult ChessGame_DoMove(ChessGame *game, ChessMove move);

/**
 * Undo the last move of a given ChessGame.
 * @param   game        the instance to undo a move on
 * @param   move        output parameter for the move that was undone,
 *                      won't change in operation failed
 * @return  CHESS_EMPTY_HISTORY if there are no moves to undo
 *          CHESS_SUCCESS otherwise (after undo a move)
 */
ChessResult ChessGame_UndoMove(ChessGame *game, ChessMove *move);

/**
 * Calculate a list of all possible moves for a given ChessPos.
 * The third argument will be redirecred to an ArrayStack* of ChessPos's
 * that should be ArrayList_Destory()'d (caller responsibility).
 * @param   game        the instance to calculate moves on
 * @param   pos         the position to calculate moves from
 * @param   moves       output parameter of the possible positions
 * @return   CHESS_INVALID_ARGUMENT if game == NULL
 *          CHESS_INVALID_POSITION is pos is not on board. In that case,
 *              the stack won't be created
 *          CHESS_EMPTY_POSITION if pos doesn't contain a player piece
 *          CHESS_SUCCESS otherwise
 */
ChessResult ChessGame_GetMoves(ChessGame *game, ChessPos pos, ArrayStack **stack);

/**
 * Retrieve a ChessColor for a given ChessPiece
 * @param   piece       the ChessPiece to retrieve the color for
 * @param   move        output parameter for the piece color
 * @return   CHESS_SUCCESS
 */
ChessResult ChessGame_GetPieceColor(ChessPiece piece, ChessColor *color);


#endif
