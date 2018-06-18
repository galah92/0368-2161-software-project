#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPFIARGame.h"

#define SP_FIAR_GAME_EDGE_SYMBOL '|'


SPFiarGame* spFiarGameCreate(int historySize) {
	SPFiarGame *game = malloc(sizeof(SPFiarGame));
	if (!game || historySize <= 0) return NULL;  // malloc failed
	// init gameBoard and tops
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			game->gameBoard[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;  // init gameBoard
			if (i == 0) game->tops[j] = 0;  // init tops
		}
	}
	game->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	game->history = spArrayListCreate(historySize);
	if (!(game->history)) return NULL;  // malloc failed
	return game;
}

SPFiarGame* spFiarGameCopy(SPFiarGame *src) {
	SPFiarGame *game = malloc(sizeof(SPFiarGame));
	if (!src || !game) return NULL; //malloc failed or game==NULL
	memcpy(game->gameBoard, src->gameBoard, sizeof(char) * SP_FIAR_GAME_N_ROWS * SP_FIAR_GAME_N_COLUMNS); //cpy the gameBoard
	memcpy(game->tops, src->tops, sizeof(int) * SP_FIAR_GAME_N_COLUMNS); //cpy the tops array
	game->currentPlayer = src->currentPlayer;
	game->history = spArrayListCopy(src->history); //cpy the history move list
	return game;
}

void spFiarGameDestroy(SPFiarGame* src) {
	if (!src) return;
	spArrayListDestroy(src->history); //free the resources of the ArrayList
	free(src);
}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col) {
	if (!src || col < 0 || col >= SP_FIAR_GAME_N_COLUMNS) return SP_FIAR_GAME_INVALID_ARGUMENT;
	// update gameBoard and tops
	if (!spFiarGameIsValidMove(src, col)) return SP_FIAR_GAME_INVALID_MOVE;
	src->gameBoard[SP_FIAR_GAME_N_ROWS - src->tops[col] - 1][col] = src->currentPlayer;
	src->tops[col] += 1;
	// update history
	if (spArrayListIsFull(src->history)) {
		spArrayListRemoveFirst(src->history);
	}
	spArrayListAddLast(src->history, col);
	// update currentPlayer
	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	} else {  // src->currentPlayer == SP_FIAR_GAME_PLAYER_2_SYMBOL
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
	return SP_FIAR_GAME_SUCCESS;
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col) {
	if (!src || col < 0 || col >= SP_FIAR_GAME_N_COLUMNS) return false; //col index illegal
	return src->tops[col] != SP_FIAR_GAME_N_ROWS;
}

SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src) {
	if (!src) return SP_FIAR_GAME_INVALID_ARGUMENT;
	if (spArrayListIsEmpty(src->history)) return SP_FIAR_GAME_NO_HISTORY;
	// update history
	int tmp = spArrayListGetLast(src -> history);
	spArrayListRemoveLast(src -> history);
	// update gameBoard and tops
	src->gameBoard[SP_FIAR_GAME_N_ROWS - (src->tops[tmp])][tmp] = SP_FIAR_GAME_EMPTY_ENTRY;
	src->tops[tmp] -= 1;
	// update currentPlayer
	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	} else {  // src->currentPlayer == SP_FIAR_GAME_PLAYER_2_SYMBOL
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
	return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src) {
	if (!src) return SP_FIAR_GAME_INVALID_ARGUMENT;
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {  // first SP_FIAR_GAME_N_ROWS rows
		printf("%c%c", SP_FIAR_GAME_EDGE_SYMBOL, SP_FIAR_GAME_EMPTY_ENTRY);  // board edge
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			printf("%c%c", src->gameBoard[i][j], SP_FIAR_GAME_EMPTY_ENTRY); // discs
		}
		printf("%c\n", SP_FIAR_GAME_EDGE_SYMBOL);  // board edge
	}
	for (int i = 0; i < (SP_FIAR_GAME_N_COLUMNS * 2) + 3; i++) printf("%c", '-');  // board bottom
	printf("\n");
	printf("%c%c", SP_FIAR_GAME_EMPTY_ENTRY, SP_FIAR_GAME_EMPTY_ENTRY); // last line
	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) printf("%d%c", i + 1, SP_FIAR_GAME_EMPTY_ENTRY);
	printf("%c\n", SP_FIAR_GAME_EMPTY_ENTRY);
	return SP_FIAR_GAME_SUCCESS;
}

char spFiarGameGetCurrentPlayer(SPFiarGame* src) {
	if (!src) return SP_FIAR_GAME_EMPTY_ENTRY;
	return src->currentPlayer;
}

char spFiarCheckWinner(SPFiarGame* src) {
	if (src == NULL) return '\0';
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS - 3; i++) { //vertical check loop
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			if (src -> gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY && //the four char equal and aren't empty
				src -> gameBoard[i][j] == src -> gameBoard[i+1][j] &&
				src -> gameBoard[i][j] == src -> gameBoard[i+2][j] &&
				src -> gameBoard[i][j] == src -> gameBoard[i+3][j]) return src -> gameBoard[i][j];
		}
	}
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) { //horizontal check loop
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			if (src -> gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY && //the four char equal and aren't empty
				src -> gameBoard[i][j] == src -> gameBoard[i][j+1] &&
				src -> gameBoard[i][j] == src -> gameBoard[i][j+2] &&
				src -> gameBoard[i][j] == src -> gameBoard[i][j+3]) return src -> gameBoard[i][j];
		}
	}
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS - 3; i++) { //diagonal (right and down)
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			if (src -> gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY && //the four char equal and aren't empty
				src -> gameBoard[i][j] == src -> gameBoard[i+1][j+1] &&
				src -> gameBoard[i][j] == src -> gameBoard[i+2][j+2] &&
				src -> gameBoard[i][j] == src -> gameBoard[i+3][j+3]) return src -> gameBoard[i][j];
		}
	}
	for (int i = SP_FIAR_GAME_N_ROWS - 3; i < SP_FIAR_GAME_N_ROWS; i++) { //diagonal (right and up)
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			if (src -> gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY && //the four char equal and aren't empty
				src -> gameBoard[i][j] == src -> gameBoard[i-1][j+1] &&
				src -> gameBoard[i][j] == src -> gameBoard[i-2][j+2] &&
				src -> gameBoard[i][j] == src -> gameBoard[i-3][j+3]) return src -> gameBoard[i][j];
		}
	}
	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		if ((src -> tops[i]) != SP_FIAR_GAME_N_ROWS) return '\0'; //game not ended
	}
	return SP_FIAR_GAME_TIE_SYMBOL;
}
