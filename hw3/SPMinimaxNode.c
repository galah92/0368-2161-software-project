#include "SPMinimaxNode.h"


/**
 *	A helper method for SPMinimaxNode_Create which init a node with no children.
 *	@param game of type SPFiarGame
 *	@return
 *	NULL if either an allocation error occurs or game == NULL,
 *  otherwise a SPMinimaxNode with no children.
 */
SPMinimaxNode* SPMinimaxNode_CreateNoChildren(SPFiarGame* game) {
	if (!game) return NULL;
	SPMinimaxNode* node = malloc(sizeof(SPMinimaxNode));
	if (!node) return NULL; //malloc failed
	node->game = spFiarGameCopy(game);
	if (!(node->game)) { //game malloc failed or game is NULL
		SPMinimaxNode_Destroy(node);
		return NULL;
	}
	node->gameIndex = -1; 
	node->children = malloc(sizeof(SPMinimaxNode) * SP_FIAR_GAME_N_COLUMNS); //children array malloc
	if (!(node->children)) { 
		SPMinimaxNode_Destroy(node);
		return NULL;
	}
	node->childrenLength = 0;
	return node;
}

SPMinimaxNode* SPMinimaxNode_Create(SPFiarGame* game, unsigned int depth, unsigned int maxDepth) {
	SPMinimaxNode *node = SPMinimaxNode_CreateNoChildren(game);
	if (depth == maxDepth || spFiarCheckWinner(node->game)) return node; //base case of minimax tree
	for (unsigned int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) { 
		if (spFiarGameIsValidMove(node->game, i)) {
			spFiarGameSetMove(node->game, i); //set move for every possible move
			node->children[node->childrenLength] = SPMinimaxNode_Create(node->game, depth + 1, maxDepth); //create the rest of the tree for every child
			if (!(node->children[node->childrenLength])) {
				SPMinimaxNode_Destroy(node);
				return NULL;
			}
			spFiarGameUndoPrevMove(node->game); //return to the origin game state
			node->children[node->childrenLength]->gameIndex = i; //update the gameIndex (represent the move) for every child
			node->childrenLength++;
		}
	}
	return node;
}

void SPMinimaxNode_Destroy(SPMinimaxNode* node) {
	if (!node) return;
	spFiarGameDestroy(node->game); //frees game resources
	for (unsigned int i = 0; i < node->childrenLength; i++) { //free resources recursivly 
		SPMinimaxNode_Destroy(node->children[i]);
	}
	free(node->children);
	free(node);
}

/**
 *	A helper method for SPMinimaxNode_GetScore which give a score
 *  for a slot given the disc in it.
 *	@param a char representing a player
 *	@return 1 if player == SP_FIAR_GAME_PLAYER_1_SYMBOL,
 *   		-1 if player == SP_FIAR_GAME_PLAYER_2_SYMBOL,
 * 			0 otherwise
 */
int GetPlayerValue(char player) {
	switch (player) {
		case SP_FIAR_GAME_PLAYER_1_SYMBOL: return 1;
		case SP_FIAR_GAME_PLAYER_2_SYMBOL: return -1;
		default: return 0;
	}
}

int SPMinimaxNode_GetScore(SPMinimaxNode* node) {
	int counters[] = { 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			if (j < 4) { //horizontal span check
				int spanScore = 0;
				for (int k = 0; k < 4; k++) { //every possible span
					spanScore += GetPlayerValue(node->game->gameBoard[i][j + k]); //+1 or -1
				}
				if (spanScore == 4) return INT_MAX; //user wins
				if (spanScore == -4) return INT_MIN; //computer wins
				counters[spanScore + 3]++;
			}
			if (i < 3) { //vertical span check
				int spanScore = 0;
				for (int k = 0; k < 4; k++) { //every possible span
					spanScore += GetPlayerValue(node->game->gameBoard[i + k][j]); //+1 or -1
				}
				if (spanScore == 4) return INT_MAX; //user wins
				if (spanScore == -4) return INT_MIN; //computer wins
				counters[spanScore + 3]++;
			}
			if (i < 3 && j < 4) { //diagonal span check
				int spanScore = 0;
				for (int k = 0; k < 4; k++) { //every possible span
					spanScore += GetPlayerValue(node->game->gameBoard[i + k][j + k]); //+1 or -1
				}
				if (spanScore == 4) return INT_MAX; //user wins
				if (spanScore == -4) return INT_MIN; //computer wins
				counters[spanScore + 3]++;
			}
			if (i < 3 && j > 2) { //diagonal span check
				int spanScore = 0;
				for (int k = 0; k < 4; k++) { //every possible span
					spanScore += GetPlayerValue(node->game->gameBoard[i + k][j - k]); //+1 or -1
				}
				if (spanScore == 4) return INT_MAX; //user wins
				if (spanScore == -4) return INT_MIN; //computer wins
				counters[spanScore + 3]++;
			}
		}
	}
	if (spFiarCheckWinner(node->game) == SP_FIAR_GAME_TIE_SYMBOL) return 0; // TIE
	return counters[0] * -5 + counters[1] * -2 + counters[2] * -1 + counters[4] + counters[5] * 2 + counters[6] * 5;
}
