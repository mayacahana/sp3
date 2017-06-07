/*
 * SPMiniMax.c
 *
 *  Created on: 2 ביוני 2017
 *      Author: uri andddd mayaaa\
 *
 */
#include "SPMiniMaxNode.h"

int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth) {
	if (!currentGame || maxDepth <= 0)
		return -1;

	SPFiarGame* copy = spFiarGameCopy(currentGame);
	if (!copy)
		return -1;

	SPMiniMaxNode* node = spMiniMaxNodeCreate(copy);
	if(!node)
		return -1;

	if(spFiarCheckOver(copy)) {
		return -1;
	}

	node->value = computeValueRec(copy, maxDepth, true);
	int indexBest = 0, tmp = 0;

	for (int col = 0; col < SP_FIAR_GAME_N_COLUMNS; col++) {
		if (spFiarGameIsValidMove(copy, col)) {
			spFiarGameSetMove(copy, col);
			tmp = computeValueRec(copy, maxDepth - 1, false);
			if (tmp == node->value) {
				indexBest = col;
				break;
			}
			spFiarGameUndoPrevMove(copy);
		}
	}
	//free memory
	return indexBest;
}

