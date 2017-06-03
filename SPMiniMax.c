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

	SPMiniMaxNode* node = SPMiniMaxNodeCreate(currentGame);
	if(!node)
		return -1;

	node->value = computeValueRec(copy, maxDepth, true);
	int indexCol = 0;
	int tmpVal = 0;
	//TODO: what if the game is over
	for (int col = 0; col < 7; col++) {
		if (spFiarGameIsValidMove(copy, col)) {
			spFiarGameSetMove(copy, col);
			tmpVal = computeValueRec(copy, maxDepth - 1, false);
			if (tmpVal == node->value) {
				indexCol = col;
				break;
			}
			spFiarGameUndoPrevMove(copy);
		}
	}
	return indexCol;
}

