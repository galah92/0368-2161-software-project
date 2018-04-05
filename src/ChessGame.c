#include <stdbool.h>
#include <string.h>
#include "ChessGame.h"

#define CHESS_HISTORY_SIZE 3
#define CHESS_MAX_POSSIBLE_MOVES 27 // 7 * 3 + 6 for a queen piece

/**
 * Init a given ChessGame's board according to chess rules.
 * White is at the "bottom" of the board - lower indexes.
 * @param   game        the instance init board to
 */
void initChessBoard(ChessGame *game) {
    game->board[0][0] = game->board[0][7] = CHESS_PIECE_WHITE_ROOK;
    game->board[0][1] = game->board[0][6] = CHESS_PIECE_WHITE_KNIGHT;
    game->board[0][2] = game->board[0][5] = CHESS_PIECE_WHITE_BISHOP;
    game->board[0][3] = CHESS_PIECE_WHITE_QUEEN;
    game->board[0][4] = CHESS_PIECE_WHITE_KING;
    game->board[7][0] = game->board[7][7] = CHESS_PIECE_BLACK_ROOK;
    game->board[7][1] = game->board[7][6] = CHESS_PIECE_BLACK_KNIGHT;
    game->board[7][2] = game->board[7][5] = CHESS_PIECE_BLACK_BISHOP;
    game->board[7][3] = CHESS_PIECE_BLACK_QUEEN;
    game->board[7][4] = CHESS_PIECE_BLACK_KING;
    for (int j = 0; j < CHESS_GRID; j++) {
        game->board[1][j] = CHESS_PIECE_WHITE_PAWN;
        game->board[6][j] = CHESS_PIECE_BLACK_PAWN;
    }
    for (int i = 2; i < CHESS_GRID - 2; i++) {
        for (int j = 0; j < CHESS_GRID; j++) {
            game->board[i][j] = CHESS_PIECE_NONE;
        }
    }
}

/**
 * Check whether a given ChessPos's location is on board.
 * @param   pos         the pos to check
 */
bool isValidPositionOnBoard(ChessPos pos) {
    return pos.x >= 0 && pos.x < CHESS_GRID && pos.y >= 0 && pos.y < CHESS_GRID;
}

/**
 * Check whether a given ChessMove's locations is on board.
 * @param   move        the move to check
 */
bool isValidPositionsOnBoard(ChessMove move) {
    return isValidPositionOnBoard(move.from) && isValidPositionOnBoard(move.to);
}

ChessColor getPieceColor(ChessPiece piece) {
    switch (piece) {
        case CHESS_PIECE_WHITE_PAWN:
        case CHESS_PIECE_WHITE_ROOK:
        case CHESS_PIECE_WHITE_KNIGHT:
        case CHESS_PIECE_WHITE_BISHOP:
        case CHESS_PIECE_WHITE_QUEEN:
        case CHESS_PIECE_WHITE_KING:
            return CHESS_PLAYER_COLOR_WHITE;
        case CHESS_PIECE_BLACK_PAWN:
        case CHESS_PIECE_BLACK_ROOK:
        case CHESS_PIECE_BLACK_KNIGHT:
        case CHESS_PIECE_BLACK_BISHOP:
        case CHESS_PIECE_BLACK_QUEEN:
        case CHESS_PIECE_BLACK_KING:
            return CHESS_PLAYER_COLOR_BLACK;
        case CHESS_PIECE_NONE:
        default:
            return CHESS_PLAYER_COLOR_NONE;
    }
}

/**
 * Check whether a given ChessMove's "from" location is of the current player,
 * according to a given game.
 * @param   game        the game the move is based on
 * @param   move        the move to check
 */
bool isMoveOfPlayerPiece(ChessGame *game, ChessMove move) {
    if (!game) return false; // sanity check
    return getPieceColor(game->board[move.from.x][move.from.y]) == game->turn;
}

/**
 * Check whether a given moving piece isn't stepping on a friendly piece.
 * A friendly piece is a piece that shares the same color as the moving piece.
 * @param   game        the game instance which provides the board
 * @param   move        the move to check it's legitimacy
 * @return              0 if the 'end' move location contains a friendly piece
 *                      1 otherwise
 */
bool isValidToPosition(ChessGame *game, ChessMove move) {
    if (!game) return false; // sanity check
    ChessColor fromColor = getPieceColor(game->board[move.from.x][move.from.y]);
    ChessColor toColor = getPieceColor(game->board[move.to.x][move.to.y]);
    return fromColor != toColor;
}

bool isValidPawnMove(ChessGame *game, ChessMove move) {
    ChessColor color = getPieceColor(game->board[move.from.x][move.from.y]);
    int isInStartPos = move.from.y == (color == CHESS_PLAYER_COLOR_WHITE ? 1 : 6);
    int horDiff = abs(move.from.x - move.to.x);
    int verDiff = (move.from.y - move.to.y) * (color == CHESS_PLAYER_COLOR_WHITE ? 1 : -1);
    int isCapture = game->board[move.to.x][move.to.y] != CHESS_PIECE_NONE &&
        color != getPieceColor(game->board[move.to.x][move.to.y]);
    int regularMove = !isCapture && verDiff == 1 && horDiff == 0;
    int startingMove = !isCapture && isInStartPos && verDiff == 2 && horDiff == 0;
    int capturingMove = !isCapture && verDiff == 1 && horDiff == 1;
    return regularMove || startingMove || capturingMove;
}

bool isValidRookMove(ChessGame *game, ChessMove move) {
    int horDiff = move.from.x - move.to.x;
    int verDiff = move.from.y - move.to.y;
    if ((horDiff != 0) ^ (verDiff != 0)) return 0; // exclusive ver / hor move
    if (horDiff != 0) { // check there isn't overleap
        int start = move.from.x < move.to.x ? move.from.x + 1 : move.to.x + 1;
        int end = move.from.x < move.to.x ? move.to.x : move.from.x;
        for (int i = start; i < end; i++) {
            if (game->board[i][move.from.y] != CHESS_PIECE_NONE) return false;
        }
    } else { // verDiff != 0
        int start = move.from.y < move.to.y ? move.from.y + 1 : move.to.y + 1;
        int end = move.from.y < move.to.y ? move.to.y : move.from.y;
        for (int i = start + 1; i < end; i++) {
            if (game->board[move.from.x][i] != CHESS_PIECE_NONE) return false;
        }
    }
    return true;
}

bool isValidKnightMove(ChessGame *game, ChessMove move) {
    (void)game; // TODO: consider remove that argument as it's unused.
    int horDiff = abs(move.from.x - move.to.x);
    int verDiff = abs(move.from.y - move.to.y);
    return (verDiff == 2 && horDiff == 1) ^ (horDiff == 2 && verDiff == 1);
}

bool isValidBishopMove(ChessGame *game, ChessMove move) {
    int horAbs = abs(move.from.x - move.to.x);
    int verAbs = abs(move.from.y - move.to.y);
    if (horAbs != verAbs) return 0;
    int startX = move.from.x < move.to.x ? move.from.x + 1 : move.to.x + 1;
    int startY = move.from.y < move.to.y ? move.from.y + 1 : move.to.y + 1;
    int endX = startX == move.from.x + 1 ? move.to.x : move.from.x;
    int endY = startY == move.from.y + 1 ? move.to.y : move.from.y;
    for (int i = startX; i < endX; i++){
        for (int j = startY; j < endY; j++){
            if (game->board[i][j] != CHESS_PIECE_NONE) return 0;
        }
    }
    return true;
}

bool isValidQueenMove(ChessGame *game, ChessMove move) {
    return isValidRookMove(game, move) || isValidBishopMove(game, move);
}

bool isValidKingMove(ChessGame *game, ChessMove move) {
    (void)game; // TODO: consider remove that argument as it's unused.
    int horDiff = abs(move.from.x - move.to.x);
    int verDiff = abs(move.from.y - move.to.y);
    return horDiff <= 1 && verDiff <= 1 && ((horDiff > 0) ^ (verDiff > 0));
}

bool isValidPieceMove(ChessGame *game, ChessMove move) {
    if (!game) return false;
    switch (game->board[move.from.x][move.from.y])
    {
        case CHESS_PIECE_WHITE_PAWN:
        case CHESS_PIECE_BLACK_PAWN:
            return isValidPawnMove(game, move);
        case CHESS_PIECE_WHITE_ROOK:
        case CHESS_PIECE_BLACK_ROOK:
            return isValidRookMove(game, move);    
        case CHESS_PIECE_WHITE_KNIGHT:
        case CHESS_PIECE_BLACK_KNIGHT:
            return isValidKnightMove(game, move);
        case CHESS_PIECE_WHITE_BISHOP:
        case CHESS_PIECE_BLACK_BISHOP:
            return isValidBishopMove(game, move);
        case CHESS_PIECE_WHITE_QUEEN:
        case CHESS_PIECE_BLACK_QUEEN:
            return isValidQueenMove(game, move);
        case CHESS_PIECE_WHITE_KING:
        case CHESS_PIECE_BLACK_KING:
            return isValidKingMove(game, move);
        case CHESS_PIECE_NONE:
        default:
            return false; // shouldn't happen
    }
}

ChessPos getKingPosition(ChessGame *game, ChessColor color){
    if (!game) return (ChessPos){ .x = -1, .y = -1 };
    ChessPiece king = color == CHESS_PLAYER_COLOR_WHITE
        ? CHESS_PIECE_WHITE_KING
        : CHESS_PIECE_BLACK_KING;
    for (int i = 0 ; i < CHESS_GRID; i++) {
        for (int j = 0; j < CHESS_GRID; j++) {
            if (game->board[i][j] == king) return (ChessPos){ .x = i, .y = j };
        }
    }
    return (ChessPos){ .x = -1, .y = -1 }; // can't happen
}

bool isKingThreatened(ChessGame *game, ChessColor playerColor) {
    ChessColor opponentColor = 3 - playerColor;
    ChessMove move = { .to = getKingPosition(game, opponentColor) };
    for (int i = 0; i < CHESS_GRID; i++) {
        for (int j = 0; i < CHESS_GRID; i++) {
            if (getPieceColor(game->board[i][j]) == playerColor) {
                move.from = (ChessPos){ .x = i, .y = j };
                if (isValidPieceMove(game, move)) return true;
            }
        }
    }
    return false;
}

void pseudoDoMove(ChessGame *game, ChessMove move) {
    if (!game) return; // sanity check
    move.capturedPiece = game->board[move.to.x][move.to.y];
    game->board[move.to.x][move.to.y] = game->board[move.from.x][move.from.y];
    game->board[move.from.x][move.from.y] = CHESS_PIECE_NONE;
    game->turn = 3 - game->turn; // elegant way to switch player
    ArrayStack_Push(game->history, &move); // update history
}

bool hasMoves(ChessGame *game) {
    ArrayStack *possibleMoves;
    ChessPos pos;
    for (int i = 0; i < CHESS_GRID; i ++) {
        for (int j = 0; j < CHESS_GRID; j++) {
            pos = (ChessPos){ .x = i, .y = j };
            if (!isValidPositionOnBoard(pos)) continue;
            ChessResult res = ChessGame_GetMoves(game, pos, &possibleMoves);
            bool areThereMoves = !ArrayStack_IsEmpty(possibleMoves);
            ArrayStack_Destroy(possibleMoves);
            if (res != CHESS_SUCCESS || areThereMoves) return true;
        }
    }
    return false;
}

ChessGame* ChessGame_Create() {
    ChessGame *game = malloc(sizeof(ChessGame));
    if (!game) return NULL;
    game->turn = CHESS_PLAYER_COLOR_WHITE;
    ChessGame_SetDefaultSettings(game);
    initChessBoard(game);
    game->history = ArrayStack_Create(CHESS_HISTORY_SIZE, sizeof(ChessMove));
    if (!game->history) {
        ChessGame_Destroy(game);
        return NULL;
    }
    return game;
}

ChessGame* ChessGame_Copy(ChessGame *game) {
    ChessGame *copy = malloc(sizeof(ChessGame));
    if (!copy) return NULL;
    memcpy(copy, game, sizeof(ChessGame));
    copy->history = ArrayStack_Copy(game->history);
    if (!copy->history) {
        ChessGame_Destroy(copy);
        return NULL;
    }
    return copy;
}

void ChessGame_Destroy(ChessGame *game) {
    if (!game) return;
    ArrayStack_Destroy(game->history);
    free(game);
}

ChessResult ChessGame_SetDefaultSettings(ChessGame *game) {
    if (!game) return CHESS_INVALID_ARGUMENT;
    game->mode = CHESS_MODE_1_PLAYER;
    game->difficulty = CHESS_DIFFICULTY_EASY;
    game->userColor = CHESS_PLAYER_COLOR_WHITE;
    return CHESS_SUCCESS;
}

ChessResult ChessGame_SetGameMode(ChessGame *game, ChessMode mode) {
    if (!game) return CHESS_INVALID_ARGUMENT;
    if (mode < CHESS_MODE_1_PLAYER) return CHESS_INVALID_ARGUMENT;
    if (mode > CHESS_MODE_2_PLAYER) return CHESS_INVALID_ARGUMENT;
    game->mode = mode;
    return CHESS_SUCCESS;
}

ChessResult ChessGame_SetDifficulty(ChessGame *game, ChessDifficulty difficulty) {
    if (!game) return CHESS_INVALID_ARGUMENT;
    if (difficulty < CHESS_DIFFICULTY_AMATEUR) return CHESS_INVALID_ARGUMENT;
    if (difficulty > CHESS_DIFFICULTY_EXPERT) return CHESS_INVALID_ARGUMENT;
    game->difficulty = difficulty;
    return CHESS_SUCCESS;
}

ChessResult ChessGame_SetUserColor(ChessGame *game, ChessColor userColor) {
    if (!game) return CHESS_INVALID_ARGUMENT;
    if (userColor < CHESS_PLAYER_COLOR_BLACK) return CHESS_INVALID_ARGUMENT;
    if (userColor > CHESS_PLAYER_COLOR_WHITE) return CHESS_INVALID_ARGUMENT;
    game->userColor = userColor;
    return CHESS_SUCCESS;
}

ChessResult ChessGame_GetGameStatus(ChessGame *game, ChessStatus *status) {
    if (!game) return CHESS_INVALID_ARGUMENT;
    if (isKingThreatened(game, game->turn)) {
        *status = hasMoves(game) ? CHESS_STATUS_CHECK : CHESS_STATUS_CHECKMATE;
    } else {
        *status = hasMoves(game) ? CHESS_STATUS_RUNNING : CHESS_STATUS_DRAW;
    }
    return CHESS_SUCCESS;
}

ChessResult ChessGame_IsValidMove(ChessGame *game, ChessMove move) {
    if (!game) return CHESS_INVALID_ARGUMENT;
    if (!isValidPositionsOnBoard(move)) return CHESS_INVALID_POSITION;
    if (!isMoveOfPlayerPiece(game, move)) return CHESS_EMPTY_POSITION;
    if (!isValidToPosition(game, move)) return CHESS_ILLEGAL_MOVE;
    if (!isValidPieceMove(game, move)) return CHESS_ILLEGAL_MOVE;
    ChessStatus currentTurnStatus, nextTurnStatus;
    ChessGame_GetGameStatus(game, &currentTurnStatus);
    pseudoDoMove(game, move);
    ChessGame_GetGameStatus(game, &nextTurnStatus);
    ChessGame_UndoMove(game);
    if (nextTurnStatus == CHESS_STATUS_CHECK) {
        if (currentTurnStatus == CHESS_STATUS_CHECK) {
            return CHESS_KING_IS_STILL_THREATENED; 
        } else {
            return CHESS_KING_WILL_BE_THREATENED;
        }
    }
    return CHESS_SUCCESS;
}

ChessResult ChessGame_DoMove(ChessGame *game, ChessMove move) {
    ChessResult isValidResult = ChessGame_IsValidMove(game, move);
    if (isValidResult != CHESS_SUCCESS) return isValidResult;
    pseudoDoMove(game, move);
    return CHESS_SUCCESS;
}

ChessResult ChessGame_UndoMove(ChessGame *game) {
    if (ArrayStack_IsEmpty(game->history)) return CHESS_EMPTY_HISTORY;
    ChessMove *move = ArrayStack_Pop(game->history);
    game->board[move->from.x][move->from.y] = game->board[move->to.x][move->to.y];
    game->board[move->to.x][move->to.y] = move->capturedPiece; 
    return CHESS_SUCCESS;
}

ChessResult ChessGame_GetMoves(ChessGame *game, ChessPos pos, ArrayStack **stack) {
    if (!game) return CHESS_INVALID_ARGUMENT;
    if (!isValidPositionOnBoard(pos)) return CHESS_INVALID_POSITION;
    *stack = ArrayStack_Create(CHESS_MAX_POSSIBLE_MOVES, sizeof(ChessPos));
    // TODO: handle *stack == null (currently there's no good error type)
    ChessMove move = { .from = pos };
    for (int i = 0; i < CHESS_GRID; i++) {
        for (int j = 0; j < CHESS_GRID; j++) {
            move.to = (ChessPos){ .x = i, .y = j };
            if (ChessGame_IsValidMove(game, move) == CHESS_SUCCESS) {
                ArrayStack_Push(*stack, &(ChessPos){ .x = i, .y = j });
            }
        }
    }
    return CHESS_SUCCESS;
}

char* colorToString(const ChessGame *game) {
    switch (game->userColor) {
        case CHESS_PLAYER_COLOR_WHITE:
            return "white";
        case CHESS_PLAYER_COLOR_BLACK:
            return "black";
        case CHESS_PLAYER_COLOR_NONE:
        default:
            return "none"; // shouldn't happen
    }
}

char *difficultyString(const ChessGame *game) {
    switch (game->difficulty) {
        case CHESS_DIFFICULTY_AMATEUR:
            return "amateur";
        case CHESS_DIFFICULTY_EASY:
            return "easy";
        case CHESS_DIFFICULTY_MODERATE:
            return "moderate";
        case CHESS_DIFFICULTY_HARD:
            return "hard";
        case CHESS_DIFFICULTY_EXPERT:
            return "expert";
        default:
            return ""; // shouldn't happen
    }
}

ChessResult ChessGame_SettingsToStream(const ChessGame *game, FILE *stream) {
    if (!game || !stream) return CHESS_INVALID_ARGUMENT;
    fprintf(stream, "SETTINGS:\n");
    if (game->mode == CHESS_MODE_1_PLAYER) {
        fprintf(stream, "GAME_MODE: 1-player\n");
        fprintf(stream, "DIFFICULTY: %s\n", difficultyString(game));
        fprintf(stream, "USER_COLOR: %s\n", colorToString(game));
    } else { // game->mode == CHESS_MODE_2_PLAYER)
        fprintf(stream, "GAME_MODE: 2-player\n");
    }
    return CHESS_SUCCESS;
}

ChessResult ChessGame_BoardToStream(const ChessGame *game, FILE *stream) {
    if (!game || !stream) return CHESS_INVALID_ARGUMENT;
    // fprintf(stream, "\n");
    for (int i = CHESS_GRID - 1; i >= 0; i--) {
        fprintf(stream, "%d| ", i + 1);
        for (int j = 0; j < CHESS_GRID; j++) {
            fprintf(stream, "%c ", game->board[i][j]);
        }
        fprintf(stream, "|\n");
    }
    fprintf(stream, "  -----------------\n");
    fprintf(stream, "   A B C D E F G H\n");
    return CHESS_SUCCESS;
}

ChessResult ChessGame_GetPieceColor(ChessPiece piece, ChessColor *color) {
    *color = getPieceColor(piece);
    return CHESS_SUCCESS;
}
