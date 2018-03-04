#ifndef CHESS_GAME_H_
#define CHESS_GAME_H_

#include "FSAStack.h"


#define CHESS_GAME_BOARD_SIZE 8

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
    CHESS_PLAYER_COLOR_BLACK = 1,
    CHESS_PLAYER_COLOR_WHITE,
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

typedef enum ChessPieceStatus {
    CHESS_PIECE_STANDARD,
    CHESS_PIECE_THREATENED,
    CHESS_PIECE_CAPTURED,
    CHESS_PIECE_THREATENED_AND_CAPTURED
} ChessPieceStatus;

typedef enum ChessPieceType {
    CHESS_PIECE_TYPE_NONE,
    CHESS_PIECE_TYPE_PAWN,
    CHESS_PIECE_TYPE_ROOK,
    CHESS_PIECE_TYPE_KNIGHT,
    CHESS_PIECE_TYPE_BISHOP,
    CHESS_PIECE_TYPE_QUEEN,
    CHESS_PIECE_TYPE_KING,
} ChessPieceType;

typedef struct ChessPiece {
    ChessPieceType type;
    ChessPieceStatus status;
    ChessPlayerColor color;
} ChessPiece;

typedef struct ChessGame {
    ChessGameStatus status;
    ChessGameSettings settings;
    ChessPiece board[CHESS_GAME_BOARD_SIZE][CHESS_GAME_BOARD_SIZE];
    ChessPlayerColor currentTurn;
    FSAStack *history;
} ChessGame;

typedef struct ChessBoardPos {
    int x;
    int y;
} ChessBoardPos;

typedef struct ChessMove {
    ChessBoardPos from;
    ChessBoardPos to;
    ChessPiece capturedPiece;
} ChessMove;


ChessGame* ChessGame_Create();

void ChessGame_Destroy(ChessGame *game);

ChessGameResult ChessGame_SetDefaultSettings(ChessGame *game);

ChessGameResult ChessGame_SetGameMode(ChessGame *game, int mode);

ChessGameResult ChessGame_SetDifficulty(ChessGame *game, int difficulty);

ChessGameResult ChessGame_SetUserColor(ChessGame *game, int userColor);

ChessGameResult ChessGame_PrintSettings(ChessGame *game);

ChessGameResult ChessGame_IsValidMove(ChessGame *game, ChessMove move);

ChessGameResult ChessGame_DoMove(ChessGame *game, ChessMove move);

ChessGameResult ChessGame_UndoMove(ChessGame *game);

ChessGameResult ChessGame_GetMoves(ChessGame *game, ChessBoardPos pos, ChessMove *moves[]);


#endif
