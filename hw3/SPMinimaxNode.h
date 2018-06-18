#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_
#include <stdlib.h>
#include <limits.h>
#include "SPFIARGame.h"

/**
 * SPMinimaxNode Summary:
 *
 * A container that represents a minimax TreeNode.
 * The container supports the following functions.
 *
 * SPMinimaxNode_Create           - Creates a new node with all it's children
 * SPMinimaxNode_Destroy          - Frees all memory resources associated with a node
 * SPMinimaxNode_GetScore         - Return the gameBoard score
 */

typedef struct SPMinimaxNode {
	SPFiarGame* game;
	unsigned int gameIndex;
	struct SPMinimaxNode** children;
	unsigned int childrenLength;
} SPMinimaxNode;

/**
 *	Creates a SPMinimaxNode node with all it's children up to given maxDepth.
 *	@param game of type SPFiarGame
 *	@return
 *	NULL if either an allocation error occurs or game == NULL,
 *  otherwise a SPMinimaxNode with all it's children up to given maxDepth.
 */
SPMinimaxNode* SPMinimaxNode_Create(SPFiarGame* game, unsigned int depth, unsigned int maxDepth);

/**
 *	Frees all memory used by node
 *	@param node of type SPMinimaxNode
 *	@return void
 */
void SPMinimaxNode_Destroy(SPMinimaxNode* node);

/**
 *	Computes the current game score of a given SPMinimaxNode's game.
 *	@param node of type SPMinimaxNode
 *	@return INT_MAX if player won, INT_MIN if player lost. otherwise, the current game score.
 */
int SPMinimaxNode_GetScore(SPMinimaxNode* node);


#endif
