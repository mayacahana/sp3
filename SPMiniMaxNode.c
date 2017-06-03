/*
 * SPMiniMaxNode.c
 *
 *  Created on: 30 במאי 2017
 *      Author: uri
 */
#include "SPMiniMaxNode.h"
#include "SPFIARGame.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

SPMiniMaxNode* spMiniMaxNodeCreate(SPFiarGame* src) {

	SPMiniMaxNode* res = (SPMiniMaxNode*) malloc(sizeof(SPMiniMaxNode));
	if (!res)
		return NULL;

	res->value = 0;
	res->game = SPFIARGameCopy(src);
	if (res->game == NULL) {
		free(res);
		return NULL;
	}
	return res;
}

void spMiniMaxNodeDestroy(SPMiniMaxNode* src) {

	if (!src)
		return;

	spFiarGameDestroy(src->game);
	free(src);
	return;

}

int scoringFunc(SPFiarGame* src) {

	if (!src)
		return 0;

	char currentPlayer = src->currentPlayer;
	int histo[7] = { 0 };
	int score = 0;
	int tmp = 0;

	//vertical check
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS - 3; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i][j]);
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i + 1][j]);
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i + 2][j]);
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i + 3][j]);
			if (tmp == 4) {
				score = INT_MAX;
				return score;
			}
			if (tmp == -4) {
				score = INT_MIN;
				return score;
			}
			//tmp E {-3, -2, -1, 0, 1, 2, 3}
			histo[tmp + 3] = histo[tmp + 3] + 1;
		}
		tmp = 0;
	}

	//horizonal check
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i][j]);
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i][j + 1]);
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i][j + 2]);
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i][j + 3]);
			if (tmp == 4) {
				score = INT_MAX;
				return score;
			}
			if (tmp == -4) {
				score = INT_MIN;
				return score;
			}
			//tmp E {-3, -2, -1, 0, 1, 2, 3}
			histo[tmp + 3] = histo[tmp + 3] + 1;
		}
		tmp = 0;
	}

	//diagonal check - ascending
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS - 3; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i][j]);
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i + 1][j + 1]);
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i + 2][j + 2]);
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i + 3][j + 3]);
			if (tmp == 4) {
				score = INT_MAX;
				return score;
			}
			if (tmp == -4) {
				score = INT_MIN;
				return score;
			}
			//tmp E {-3, -2, -1, 0, 1, 2, 3}
			histo[tmp + 3] = histo[tmp + 3] + 1;
		}
		tmp = 0;
	}
	//diagonal check - descending
	for (int i = 3; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i][j]);
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i - 1][j + 1]);
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i - 2][j + 2]);
			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i - 3][j + 3]);
			if (tmp == 4) {
				score = INT_MAX;
				return score;
			}
			if (tmp == -4) {
				score = INT_MIN;
				return score;
			}
			//tmp E {-3, -2, -1, 0, 1, 2, 3}
			histo[tmp + 3] = histo[tmp + 3] + 1;
		}
		tmp = 0;
	}
	//histo[3] represents 0's spans, ignore it
	//weighting the score
	score = (-5) * histo[0] + (-2) * histo[1] + (-1) * histo[2] + (1) * histo[4]
			+ (2) * histo[5] + (5) * histo[6];
	return score;
}

int getNumericValue(char currentPlayer, char symbol) {
	if (symbol == currentPlayer)
		return 1;
	if (symbol == SP_FIAR_GAME_EMPTY_ENTRY)
		return 0;
	//the symbol is the opposite symbol
	return -1;
}

int computeValueRec(SPFiarGame* src, int maxRecLvl, bool flag) {

	//recursion halt
	if (maxRecLvl == 0)
		return scoringFunc(src);

	//TODO: check
	int value =  spFiarCheckOver(src) ? scoringFunc(src) : ((flag) ? INT_MIN : INT_MAX);
	for (int col = 0; col < 7; col++) {
		if (spFiarGameIsValidMove(src, col)) {
			spFiarGameSetMove(src, col);
			value = decider(value, computeValueRec(src, maxRecLvl - 1, !flag), flag);
			spFiarGameUndoPrevMove(src);
		} else
			continue;
	}
	return value;
}

int decider(int value, int curr, bool flag) {
	//if the flag equals TRUE, take a MAX between elems
	if (flag)
		return MAX(value, curr);
	else
		return MIN(value, curr);
}


