#include "SPMinimaxNode.h"


 /**
  * Evaluates the next best move using the MiniMax algorithm up to given maxDepth.
  * @param node - The current game state node
  * @param depth - The current depth of the game state node
  * @param maxDepth - The maximum depth of the miniMax algorithm
  * @param isMaximizingPlayer - The goal of current player
  * @return an integer representing best move according to minimax
  */
int minimax(SPMinimaxNode* node, unsigned int depth, unsigned int maxDepth, int isMaximizingPlayer) {
	if (depth == maxDepth || node->childrenLength == 0) return SPMinimaxNode_GetScore(node); //base case of the algorithm
	int bestScore = minimax(node->children[0], depth + 1, maxDepth, !isMaximizingPlayer); //recursive call for the first child
  int bestScoreIndex = node->children[0]->gameIndex; 
  //recursive call for every child and updating bestScore and bestScoreIndex
	for (unsigned int i = 1; i < node->childrenLength; i++) { 
		int score = minimax(node->children[i], depth + 1, maxDepth, !isMaximizingPlayer);
        if ((isMaximizingPlayer && score > bestScore) || (!isMaximizingPlayer && score < bestScore)) {
            bestScore = score;
            bestScoreIndex = node->children[i]->gameIndex;
        }
    }
	return depth == 0 ? bestScoreIndex : bestScore;
}

int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth) {
	if (!currentGame || maxDepth <= 0) return -1; //illegal call
	SPMinimaxNode *node = SPMinimaxNode_Create(currentGame, 0, maxDepth); //create the minimax tree
	if (!node) return -1;
  int result = minimax(node, 0, maxDepth, currentGame->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL); //result of minimax
	SPMinimaxNode_Destroy(node);
	return result;
}
