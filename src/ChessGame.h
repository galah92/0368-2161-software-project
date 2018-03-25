#ifndef CHESS_GAME_H_
#define CHESS_GAME_H_

#include "FSAStack.h"


#define CHESS_GAME_BOARD_SIZE 8
#define CHESS_GAME_HISTORY_SIZE 3

typedef enum ChessGameResult {
    CHESS_GAME_SUCCESS,
    CHESS_GAME_INVALID_ARGUMENT,
    CHESS_GAME_INVALID_POSITION,
    CHESS_GAME_EMPTY_POSITION,
    CHESS_GAME_ILLEGAL_MOVE,
    CHESS_GAME_KING_IS_STILL_THREATENED,
    CHESS_GAME_KING_WILL_BE_THREATENED,
} ChessGameResult;

typedef enum ChessGameMode {
    CHESS_GAME_MODE_1_PLAYER = 1,
    CHESS_GAME_MODE_2_PLAYER,
} ChessGameMode;

typedef enum ChessGameDifficulty {
    CHESS_GAME_DIFFICULTY_AMATEUR = 1,
    CHESS_GAME_DIFFICULTY_EASY,
    CHESS_GAME_DIFFICULTY_MODERATE,
    CHESS_GAME_DIFFICULTY_HARD,
    CHESS_GAME_DIFFICULTY_EXPERT,
} ChessGameDifficulty;

typedef enum ChessPlayerColor {
    CHESS_PLAYER_COLOR_WHITE,
    CHESS_PLAYER_COLOR_BLACK,
} ChessPlayerColor;

typedef struct ChessGameSettings {
    ChessGameMode mode;
    ChessGameDifficulty difficulty;
    ChessPlayerColor userColor;
} ChessGameSettings;

typedef enum ChessGameStatus {
    CHS_RUNNING,
    CHS_PLAYER1_WINNER,
    CHS_PLAYER2_WINNER,
    CHS_TIE,
} ChessGameStatus;

typedef enum ChessTileType {
    CHESS_PIECE_TYPE_NONE,
    CHESS_PIECE_TYPE_PAWN,
    CHESS_PIECE_TYPE_ROOK,
    CHESS_PIECE_TYPE_KNIGHT,
    CHESS_PIECE_TYPE_BISHOP,
    CHESS_PIECE_TYPE_QUEEN,
    CHESS_PIECE_TYPE_KING,
} ChessTileType;

typedef struct ChessTile {
    ChessTileType type;
    ChessPlayerColor color;
} ChessTile;

typedef struct ChessGame {
    ChessGameStatus status;
    ChessGameSettings settings;
    ChessTile board[CHESS_GAME_BOARD_SIZE][CHESS_GAME_BOARD_SIZE];
    ChessPlayerColor currentTurn;
    int isWhiteKingThreatened;
    int isBlackKingThreatened;
    FSAStack *history;
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
ChessGameResult ChessGame_SetDefaultSettings(ChessGame *game);

/**
 * Set a given mode to a given game instance.
 * @param   game        the instance to set mode to
 * @param   mode        the mode to set
 */
ChessGameResult ChessGame_SetGameMode(ChessGame *game, ChessGameMode mode);

/**
 * Set a given difficulty to a given game instance.
 * @param   game        the instance to set mode to
 * @param   difficulty  the difficulty to set
 */
ChessGameResult ChessGame_SetDifficulty(ChessGame *game, ChessGameDifficulty difficulty);

/**
 * Set a given user color to a given game instance.
 * @param   game        the instance to set mode to
 * @param   userColor   the difficulty to set
 */
ChessGameResult ChessGame_SetUserColor(ChessGame *game, ChessPlayerColor userColor);

ChessGameResult ChessGame_IsValidMove(ChessGame *game, ChessMove move);

ChessGameResult ChessGame_DoMove(ChessGame *game, ChessMove move);

ChessGameResult ChessGame_UndoMove(ChessGame *game);

ChessGameResult ChessGame_GetMoves(ChessGame *game, ChessBoardPos pos, ChessMove *moves[]);


#endif
