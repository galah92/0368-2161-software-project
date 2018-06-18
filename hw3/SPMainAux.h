#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPFIARGame.h"
#include "SPFIARParser.h"
#include "SPMinimax.h"

//Definitions

#define GAME_HISTORY_SIZE 20

#define MESSAGE_ERR_MALLOC_FAILED "Error: malloc has failed\n"
#define MESSAGE_ERR_LEVEL "Error: invalid level (should be between 1 to 7)\n"
#define MESSAGE_ERR_INVALID_COMMAND "Error: invalid command\n"
#define MESSAGE_ERR_COLUMN_RANGE "Error: column number must be in range 1-7\n"
#define MESSAGE_ERR_COLUMN_FULL "Error: column %d is full\n"
#define MESSAGE_ERR_PREVIOUS_MOVE "Error: cannot undo previous move!\n"
#define MESSAGE_ERR_GAME_IS_OVER "Error: the game is over\n"

#define MESSAGE_EXITING "Exiting...\n"
#define MESSAGE_RESTARTED "Game restarted!\n"
#define MESSAGE_DIFFIULTY "Please enter the difficulty level between [1-7]:\n"
#define MESSAGE_SUGGESTED_MOVE "Suggested move: drop a disc to column %d\n"
#define MESSAGE_REMOVE_PC_DISC "Remove disc: remove computer's disc at column %d\n"
#define MESSAGE_REMOVE_USER_DISC "Remove disc: remove user's disc at column %d\n"
#define MESSAGE_PC_MOVE "Computer move: add disc to column %d\n"

#define MESSAGE_MAKE_NEXT_MOVE "Please make the next move:\n"
#define MESSAGE_USER_WON "Game over: you win\nPlease enter 'quit' to exit or 'restart' to start a new game!\n"
#define MESSAGE_COMPUTER_WON "Game over: computer wins\nPlease enter 'quit' to exit or 'restart' to start a new game!\n"
#define MESSAGE_TIE "Game over: it's a tie\nPlease enter 'quit' to exit or 'restart' to start a new game!\n"


/**
 * Type used for returning game states
 */
typedef enum GameState {
    GAME_STATE_RUNNING,
    GAME_STATE_USER_WON,
    GAME_STATE_COMPUTER_WON,
    GAME_STATE_TIE,
    GAME_STATE_QUIT,
    GAME_STATE_RESTARTED,
    GAME_STATE_NO_MOVE,
} GameState;

 /**
  * the function initialize a FIAR game, the computer's turn is determined by the minimax algorithm.
  * the function get the difficulty value from the user by MainAux_GetCommand
  * @param game - a SPFiarGame pointer
  * @param difficulty - pointer to the difficulty level
  * @return GameState as :
  *     GAME_STATE_RUNNING - if the game initilized correctly
  *		  GAME_STATE_QUIT - if malloc failed or if the user enter quit command
  *
  */
GameState InitGame(SPFiarGame **game, int *difficulty);

/**
 *  the function get FIAR game, a state and a difficulty level and process the command.
 *  the function prints messages according to the relevant command and to the state of the game.
 *  @param game - a SPFiarGame pointer
 *  @param state - a GameState state
 *  @param difficulty - a difficulty level
 *  @return GameState as:
 *      GAME_STATE_NO_MOVE if the command illegal or game is over or undo_move failed or add_disc x and x column is full
 *      GAME_STATE_QUIT if the user quit the game
 *      GAME_STATE_RESTARTED if the user restart the game
 *      GAME_STATE_USER_WON if the user add disc and won the game
 *      GAME_STATE_TIE if the game over and there is a TIE
 *      GAME_STATE_COMPUTER_WON the computer add disc and won the game
 *      GAME_STATE_RUNNING otherwise
 *
 */
GameState ProcessCommand(SPFiarGame *game, GameState state, int difficulty);

 /**
  * the function get FIAR game and a current state, and handle what should be print to the user.
  * if state equal to GAME_STATE_RUNNING or GAME_STATE_USER_WON or GAME_STATE_COMPUTER_WON or GAME_STATE_TIE, the function print
  * the game Board and a message. if stat equal to GAME_STATE_QUIT or GAME_STATE_RESTARTED the function print only a message.
  * if state equal to GAME_STATE_NO_MOVE, the function do nothing.
  * @param game - a SPFiarGame pointer
  * @param state - a GameState state
  *
  */
void PrintGameState(SPFiarGame *game, GameState state);


#endif
