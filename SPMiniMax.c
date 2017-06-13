/*
 * SPMiniMax.c
 *
 *  Created on: 2 ביוני 2017
 *      Author: uri andddd mayaaa\
 *
 */
#include "SPMiniMaxNode.h"
#include <stdlib.h>
#include <limits.h>

int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth) {
	if (!currentGame || maxDepth <= 0)
		return -1;

	int childValue = 0;
	int max = INT_MIN; //first iteration - max
	int bestIndex = -1;
	char currentPlayer = spFiarGameGetCurrentPlayer(currentGame);
	SPFiarGame* copy = spFiarGameCopy(currentGame);
	if (!copy)
		return -1;

	if (spFiarCheckOver(copy)) {
		free(copy);
		return -1;
	}
	for (int col = 0; col < SP_FIAR_GAME_N_COLUMNS; col++) {
		if (spFiarGameIsValidMove(copy, col)) {
			spFiarGameSetMove(copy, col);
			childValue = computeValueRec(copy, maxDepth - 1, false, currentPlayer);
			if (max < childValue || bestIndex == -1) {
				max = childValue;
				bestIndex = col;
			}
			spFiarGameUndoPrevMove(copy);
		}
	}
	free(copy);
	return bestIndex;
}

