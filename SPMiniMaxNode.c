/*
 * SPMiniMaxNode.c
 *
 *  Created on: 30 במאי 2017
 *      Author: uri
 */
#include "SPMiniMaxNode.h"
#include <limits.h>
//#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "SPFIARGame.h"

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

SPMiniMaxNode* spMiniMaxNodeCreate(SPFiarGame* src) {

	SPMiniMaxNode* res = (SPMiniMaxNode*) malloc(sizeof(SPMiniMaxNode));
	if (!res)
		return NULL;

	res->value = 0;
	res->game = spFiarGameCopy(src);
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
//
//int scoringFunc(SPFiarGame* src, char currentPlayer) {
//
//	if (!src)
//		return 0;
//
//	int histo[7] = { 0 };
//	int score = 0;
//	int tmp = 0;
//
//	//vertical check
//	for (int i = 0; i < SP_FIAR_GAME_N_ROWS - 3; i++) {
//		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
//			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i][j]);
//			tmp = tmp
//					+ getNumericValue(currentPlayer, src->gameBoard[i + 1][j]);
//			tmp = tmp
//					+ getNumericValue(currentPlayer, src->gameBoard[i + 2][j]);
//			tmp = tmp
//					+ getNumericValue(currentPlayer, src->gameBoard[i + 3][j]);
//			if (tmp == 4) {
//				score = INT_MAX;
//				return score;
//			}
//			if (tmp == -4) {
//				score = INT_MIN;
//				return score;
//			}
//			//tmp E {-3, -2, -1, 0, 1, 2, 3}
//			histo[tmp + 3] = histo[tmp + 3] + 1;
//			tmp = 0;
//		}
//	}
//
//	//horizonal check
//	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
//		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
//
//			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i][j]);
//			tmp = tmp
//					+ getNumericValue(currentPlayer, src->gameBoard[i][j + 1]);
//			tmp = tmp
//					+ getNumericValue(currentPlayer, src->gameBoard[i][j + 2]);
//			tmp = tmp
//					+ getNumericValue(currentPlayer, src->gameBoard[i][j + 3]);
//			if (tmp == 4) {
//				score = INT_MAX;
//				return score;
//			}
//			if (tmp == -4) {
//				score = INT_MIN;
//				return score;
//			}
//			//tmp E {-3, -2, -1, 0, 1, 2, 3}
//			histo[tmp + 3] = histo[tmp + 3] + 1;
//			tmp = 0;
//		}
//	}
//
//	//diagonal check - ascending
//	for (int i = 0; i < SP_FIAR_GAME_N_ROWS - 3; i++) {
//		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
//			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i][j]);
//			tmp = tmp
//					+ getNumericValue(currentPlayer,
//							src->gameBoard[i + 1][j + 1]);
//			tmp = tmp
//					+ getNumericValue(currentPlayer,
//							src->gameBoard[i + 2][j + 2]);
//			tmp = tmp
//					+ getNumericValue(currentPlayer,
//							src->gameBoard[i + 3][j + 3]);
//			if (tmp == 4) {
//				score = INT_MAX;
//				return score;
//			}
//			if (tmp == -4) {
//				score = INT_MIN;
//				return score;
//			}
//			//tmp E {-3, -2, -1, 0, 1, 2, 3}
//			histo[tmp + 3] = histo[tmp + 3] + 1;
//			tmp = 0;
//		}
//	}
//	//diagonal check - descending
//	for (int i = 3; i < SP_FIAR_GAME_N_ROWS; i++) {
//		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
//			tmp = tmp + getNumericValue(currentPlayer, src->gameBoard[i][j]);
//			tmp = tmp
//					+ getNumericValue(currentPlayer,
//							src->gameBoard[i - 1][j + 1]);
//			tmp = tmp
//					+ getNumericValue(currentPlayer,
//							src->gameBoard[i - 2][j + 2]);
//			tmp = tmp
//					+ getNumericValue(currentPlayer,
//							src->gameBoard[i - 3][j + 3]);
//			if (tmp == 4) {
//				score = INT_MAX;
//				return score;
//			}
//			if (tmp == -4) {
//				score = INT_MIN;
//				return score;
//			}
//			//tmp E {-3, -2, -1, 0, 1, 2, 3}
//			histo[tmp + 3] = histo[tmp + 3] + 1;
//			tmp = 0;
//		}
//	}
//	//histo[3] represents 0's spans, ignore it
//	//weighting the score
//	score = (-5) * histo[0] + (-2) * histo[1] + (-1) * histo[2] + (1) * histo[4]
//			+ (2) * histo[5] + (5) * histo[6];
//	return score;
//}

static int constVector[SP_FIAR_GAME_N_COLUMNS] = {-5,-2,-1,0,1,2,5};
int scoringFunc(SPFiarGame* currentGame, char whoCalledMe){
	int spans[2*SP_FIAR_GAME_SPAN-1] = {0,0,0,0,0,0,0};
	int cnt1 = 0;
	int cnt2 = 0;
	int cnt3 = 0;
	int cnt4 = 0;
	char entry;
	for(int row = 0; row < SP_FIAR_GAME_N_ROWS; ++row){
		for(int col = 0; col < SP_FIAR_GAME_N_COLUMNS; ++col){
			for(int j = 0; j < 4; ++j){
				//passing rows
				if(col < SP_FIAR_GAME_N_COLUMNS - 3) {
					entry = currentGame->gameBoard[row][col + j];
					if (entry != SP_FIAR_GAME_EMPTY_ENTRY)
						cnt1 = (entry == whoCalledMe) ? cnt1 + 1 : cnt1 - 1;
				}
				//passing columns
				if(row < SP_FIAR_GAME_N_ROWS - 3) {
					entry = currentGame->gameBoard[row + j][col];
					if (entry != SP_FIAR_GAME_EMPTY_ENTRY)
						cnt2 = (entry == whoCalledMe) ? cnt2 + 1 : cnt2 - 1;
				}
				//passing diagonal spans
				if(col < SP_FIAR_GAME_N_COLUMNS - 3 &&(row < SP_FIAR_GAME_N_ROWS - 3)){
					//passing ascending diagonal spans
					entry = currentGame->gameBoard[row + j][col + j];
					if (entry != SP_FIAR_GAME_EMPTY_ENTRY)
						cnt3 = (entry == whoCalledMe) ? cnt3 + 1 : cnt3 - 1;

					//passing descending diagonal spans
					entry = currentGame->gameBoard[row + 3 - j][col + j];
					if (entry != SP_FIAR_GAME_EMPTY_ENTRY)
						cnt4 = (entry == whoCalledMe) ? cnt4 + 1 : cnt4 - 1;
				}
			}
			spans[cnt1 + 3] += 1;
			spans[cnt2 + 3] += 1;
			spans[cnt3 + 3] += 1;
			spans[cnt4 + 3] += 1;
			cnt1 = 0;
			cnt2 = 0;
			cnt3 = 0;
			cnt4 = 0;
		}
	}
	int d = multiplyVector(spans,constVector,2*SP_FIAR_GAME_SPAN-1);
	return d;
}

int multiplyVector(int spans[],int vector[], int size){
	int sum = 0;
	for (int i = 0; i < size; ++i) sum += spans[i]*vector[i];
	return sum;
}
int getNumericValue(char currentPlayer, char symbol) {

	if (symbol == currentPlayer)
		return 1;
	if (symbol == SP_FIAR_GAME_EMPTY_ENTRY)
		return 0;
	//the symbol is the opposite from the current player symbol
	return -1;
}

int computeValueRec(SPFiarGame* src, int maxRecLvl, bool flag, char currentPlayer) {
	char winner = spFiarCheckWinner(src);
	if (winner != '\0') {
		if (winner == SP_FIAR_GAME_PLAYER_1_SYMBOL
				|| winner == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
			return (winner == currentPlayer) ? INT_MAX : INT_MIN;
		} else {
			return scoringFunc(src,currentPlayer);
		}
	}

	//recursion halt
	if (maxRecLvl == 0 )
		return scoringFunc(src, currentPlayer);

	if (spFiarCheckOver(src))
		return 0;

	int value = (flag) ? INT_MIN : INT_MAX;

	for (int col = 0; col < SP_FIAR_GAME_N_COLUMNS; col++) {
		printf("col:%d\n",col);
		if (spFiarGameIsValidMove(src, col)) {
			printf("salami\n");
			spFiarGameSetMove(src, col);
			//debug
			printf("current player:%c",src->currentPlayer);
			value = decider(value, computeValueRec(src, maxRecLvl - 1, !flag, currentPlayer), flag);
			spFiarGameUndoPrevMove(src);
		}
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

