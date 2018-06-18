#include "SPMainAux.h"


GameState InitGame(SPFiarGame **game, int *difficulty) {
    char buffer[SP_MAX_LINE_LENGTH];
    while (1) {
        printf(MESSAGE_DIFFIULTY);
        fgets(buffer, SP_MAX_LINE_LENGTH, stdin); //get input frum user
        if (spParserIsInt(buffer)) {
            *difficulty = atoi(buffer);
            if (*difficulty >= 1 && *difficulty <= 7) { //difficulty between 1 to 7
                *game = spFiarGameCreate(GAME_HISTORY_SIZE);
                if (!game) {
                    printf(MESSAGE_ERR_MALLOC_FAILED);
                    return GAME_STATE_QUIT;
                }
                return GAME_STATE_RUNNING;
            }
        } 
        if (!strcmp(buffer,"\n")) { //the input != new line
            printf(MESSAGE_ERR_LEVEL);
            continue;
        } 
        SPCommand sp = spParserPraseLine(buffer); //parse the command
        if (sp.cmd == SP_QUIT) return GAME_STATE_QUIT;
        printf(MESSAGE_ERR_LEVEL);
    }
}

/**
  * the function get FIARGame, a command and difficulty for the game, and handle the next move of the user or the computer.
  * if the command has illegal value, it prints "Error: invalid command\n"
  * otherwise, the funtion try to complete the next move. if the column is full, the function prints "Error: column %d is full\n"
  * if the command arg is illegal the function prints "Error: column number must be in range 1-7\n"
  * @param game - a SPFiarGame 
  * @param command - the required command
  * @param difficulty - integer between 1 to 7
  * @return GameState as :
  * 	GAME_STATE_RUNNING_NO_MESSAGE - if the column is full or the column number is illegal
  *		GAME_STATE_USER_WON - if the command succeded and the game is over (user or computer win or tie)
  * 	GAME_STATE_RUNNING - else
  */
GameState ProcessAddDisc(SPFiarGame *game, int difficulty, SPCommand command) {
    switch (spFiarGameSetMove(game, command.arg - 1)) { //try to apply the move
        case SP_FIAR_GAME_NO_HISTORY:
            return GAME_STATE_NO_MOVE;  // can't happen
        case SP_FIAR_GAME_INVALID_ARGUMENT:
            command.validArg ? printf(MESSAGE_ERR_COLUMN_RANGE) : printf(MESSAGE_ERR_INVALID_COMMAND); //col num not in range or not a number
            return GAME_STATE_NO_MOVE;
        case SP_FIAR_GAME_INVALID_MOVE:
            printf(MESSAGE_ERR_COLUMN_FULL, command.arg);
            return GAME_STATE_NO_MOVE;
        case SP_FIAR_GAME_SUCCESS:
            switch (spFiarCheckWinner(game)) {
                case SP_FIAR_GAME_PLAYER_1_SYMBOL:
                    return GAME_STATE_USER_WON;
                case SP_FIAR_GAME_TIE_SYMBOL:
                    return GAME_STATE_TIE;
            }
            int col = spMinimaxSuggestMove(game, difficulty); //pc move logic
            spFiarGameSetMove(game , col);
            printf(MESSAGE_PC_MOVE, col + 1);
            switch (spFiarCheckWinner(game)) {
                case SP_FIAR_GAME_PLAYER_2_SYMBOL:
                    return GAME_STATE_COMPUTER_WON;
                case SP_FIAR_GAME_TIE_SYMBOL:
                    return GAME_STATE_TIE;
            }
            return GAME_STATE_RUNNING;
    }
    return GAME_STATE_RUNNING;
}

/**
  * Get FIARGame and do undo move if possible. the max number of undo moves is determined by the game.
  * the function prints "Error: cannot undo previous move!\n" in case of failure,
  * and "Remove disc: remove computer's disc at column %d\n" and "Remove disc: remove user's disc at column %d\n", in case of success.
  * @param game - a SPFiarGame 
  * @return 1 if successful else 0
  */
int ProcessUndoMove(SPFiarGame *game) {
    int col = spArrayListGetLast(game->history); //get the last move
	SP_FIAR_GAME_MESSAGE msg = spFiarGameUndoPrevMove(game);
	if (msg == SP_FIAR_GAME_NO_HISTORY) { //last move isn't exist
		printf(MESSAGE_ERR_PREVIOUS_MOVE);
		return 0;
	}
    if (game->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
        printf(MESSAGE_REMOVE_USER_DISC, col + 1);
    } else {
        printf(MESSAGE_REMOVE_PC_DISC, col + 1);
    }
    return 1;
}

GameState ProcessCommand(SPFiarGame *game, GameState state, int difficulty) {
    char buffer[SP_MAX_LINE_LENGTH];
    SPCommand command = spParserPraseLine(fgets(buffer, SP_MAX_LINE_LENGTH, stdin)); //parse the input command from the user
    switch (command.cmd) {
        case SP_INVALID_LINE:
            printf(MESSAGE_ERR_INVALID_COMMAND);
            return GAME_STATE_NO_MOVE;
        case SP_QUIT:
            if (game) spFiarGameDestroy(game); //frees resources
            return GAME_STATE_QUIT;
        case SP_RESTART:
            spFiarGameDestroy(game);
            return GAME_STATE_RESTARTED;
        case SP_ADD_DISC:
            if (state == GAME_STATE_USER_WON || state == GAME_STATE_COMPUTER_WON || state == GAME_STATE_TIE) { //game over
                printf(MESSAGE_ERR_GAME_IS_OVER);
                return GAME_STATE_NO_MOVE;
            }
            return ProcessAddDisc(game, difficulty, command); //game still running
        case SP_SUGGEST_MOVE:
            if (state == GAME_STATE_USER_WON || state == GAME_STATE_COMPUTER_WON || state == GAME_STATE_TIE) { //game over
                printf(MESSAGE_ERR_GAME_IS_OVER);
            } else {
                printf(MESSAGE_SUGGESTED_MOVE, spMinimaxSuggestMove(game, difficulty) + 1);
            }
            return GAME_STATE_NO_MOVE;
        case SP_UNDO_MOVE:
            if (!ProcessUndoMove(game)) return GAME_STATE_NO_MOVE;
            if (state != GAME_STATE_USER_WON) {  // should undo pc move as well
                if (!ProcessUndoMove(game)) return GAME_STATE_NO_MOVE;
            }
            return GAME_STATE_RUNNING;
    }
    return GAME_STATE_RUNNING;
}

void PrintGameState(SPFiarGame *game, GameState state) {
    switch (state) {
        case GAME_STATE_RUNNING:
            spFiarGamePrintBoard(game);
            printf(MESSAGE_MAKE_NEXT_MOVE);
            break;
        case GAME_STATE_USER_WON:
            spFiarGamePrintBoard(game);
            printf(MESSAGE_USER_WON);
            break;
        case GAME_STATE_COMPUTER_WON:
            spFiarGamePrintBoard(game);
            printf(MESSAGE_COMPUTER_WON);
            break;
        case GAME_STATE_TIE:
            spFiarGamePrintBoard(game);
            printf(MESSAGE_TIE);
            break;
        case GAME_STATE_QUIT:
            printf(MESSAGE_EXITING);
            break;
        case GAME_STATE_RESTARTED:
            printf(MESSAGE_RESTARTED);
            break;
        case GAME_STATE_NO_MOVE:
            break;
    }
}
