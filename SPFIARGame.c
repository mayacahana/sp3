/*
 * SPFIARGame.c
 *
 *  Created on: 28 במאי 2017
 *      Author: uri
 */

#include <stdio.h>
#include <string.h>
//#include <stdbool.h>
#include <stdlib.h>
#include "SPFIARGame.h"

//Definitions
#define SP_FIAR_GAME_SPAN 4
#define SP_FIAR_GAME_N_ROWS 6
#define SP_FIAR_GAME_N_COLUMNS 7
#define SP_FIAR_GAME_PLAYER_1_SYMBOL 'X'
#define SP_FIAR_GAME_PLAYER_2_SYMBOL 'O'
#define SP_FIAR_GAME_TIE_SYMBOL '-'
#define SP_FIAR_GAME_EMPTY_ENTRY ' '

SPFiarGame* spFiarGameCreate(int historySize) {

	if (historySize <= 0)
		return NULL;

	SPFiarGame* res = (SPFiarGame*) malloc(sizeof(SPFiarGame));
	if (!res) {
		printf("Error: spFiarGameCreate has failed\n");
		exit(1);
	}

	//init board with blanks
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++)
			res->gameBoard[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;
	}

	//init tops with 0's
	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		res->tops[i] = 0;
	}

	//init current player, assuming player 1 starts the game
	res->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;

	//init movesLists
	res->movesPlayer1 = spArrayListCreate(historySize / 2);
	res->movesPlayer2 = spArrayListCreate(historySize / 2);

//	if (!res->movesPlayer1 || !res->movesPlayer2) {
//		free(res);
//		return NULL;
//	}
	return res;
}

SPFiarGame* spFiarGameCopy(SPFiarGame* src) {
	if (!src)
		return NULL;

	SPFiarGame* res = spFiarGameCreate(2 * src->movesPlayer1->maxSize);
	//SPFiarGame* res = (SPFiarGame*) malloc(sizeof(SPFiarGame));
	if (!res) {
		free(src);
		printf("Error: spFiarGameCopy has failed\n");
		exit(1);
	}
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++)
			res->gameBoard[i][j] = src->gameBoard[i][j];
	}

	//copy tops
	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
		res->tops[i] = src->tops[i];

	//copy currentPlayer
	res->currentPlayer = src->currentPlayer;

	spArrayListDestroy(res->movesPlayer1);
	spArrayListDestroy(res->movesPlayer2);
	//copy movesPlayer
	res->movesPlayer1 = spArrayListCopy(src->movesPlayer1);
	if (res->movesPlayer1 == NULL) {
		//free(res->gameBoard);
		//free(res->tops);
		free(res);
		return NULL;
	}
	res->movesPlayer2 = spArrayListCopy(src->movesPlayer2);
	if (res->movesPlayer2 == NULL) {
		//free(res->gameBoard);
		//free(res->tops);
		free(res);
		return NULL;
	}
	return res;

}

void spFiarGameDestroy(SPFiarGame* src) {
	if (src != NULL) {
		spArrayListDestroy(src->movesPlayer1);
		spArrayListDestroy(src->movesPlayer2);
		free(src);
	}

}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col) {
	if (!src || col < 0 || col > SP_FIAR_GAME_N_COLUMNS - 1)
		return SP_FIAR_GAME_INVALID_ARGUMENT;

	if (src->tops[col] == SP_FIAR_GAME_N_ROWS)
		return SP_FIAR_GAME_INVALID_MOVE;

	src->gameBoard[src->tops[col]][col] = src->currentPlayer;
	src->tops[col] = src->tops[col] + 1;

	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		if (spArrayListIsFull(src->movesPlayer1))
			spArrayListRemoveLast(src->movesPlayer1);
		spArrayListAddFirst(src->movesPlayer1, col);

	} else { //currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL
		if (spArrayListIsFull(src->movesPlayer2))
			spArrayListRemoveLast(src->movesPlayer2);
		spArrayListAddFirst(src->movesPlayer2, col);
	}
	changePlayer(src);
	return SP_FIAR_GAME_SUCCESS;
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col) {
	if (!src || col < 0|| col > SP_FIAR_GAME_N_COLUMNS - 1 ||
	src->tops[col] == SP_FIAR_GAME_N_ROWS)
		return false;
	return true;
}

SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src) {
	if (!src)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	int col = 0;

	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		if (spArrayListIsEmpty(src->movesPlayer2))
			return SP_FIAR_GAME_NO_HISTORY;

		col = spArrayListGetFirst(src->movesPlayer2);
		spArrayListRemoveFirst(src->movesPlayer2);
	} else {
		if (spArrayListIsEmpty(src->movesPlayer1))
			return SP_FIAR_GAME_NO_HISTORY;

		col = spArrayListGetFirst(src->movesPlayer1);
		spArrayListRemoveFirst(src->movesPlayer1);
	}
	changePlayer(src);
	src->gameBoard[src->tops[col] - 1][col] = SP_FIAR_GAME_EMPTY_ENTRY;
	src->tops[col] = src->tops[col] - 1;
	return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src) {
	if (!src)
		return SP_FIAR_GAME_INVALID_ARGUMENT;

	for (int i = SP_FIAR_GAME_N_ROWS - 1; i >= 0; i--) {
		printf("| ");
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++)
			printf("%c ", src->gameBoard[i][j]);
		printf("|\n");
	}
	printf("-----------------\n");
	printf("  1 2 3 4 5 6 7  \n");

	return SP_FIAR_GAME_SUCCESS;

}

char spFiarGameGetCurrentPlayer(SPFiarGame* src) {
	//if (!src)
	//	SP_FIAR_GAME_EMPTY_ENTRY;

	return src->currentPlayer;
}

char spFiarCheckWinner(SPFiarGame* src) {
	if (!src)
		return '\0';

	//vertical check
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS - 3; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			if (src->gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY
					&& (src->gameBoard[i][j] == src->gameBoard[i + 1][j])
					&& (src->gameBoard[i + 1][j] == src->gameBoard[i + 2][j])
					&& (src->gameBoard[i + 2][j] == src->gameBoard[i + 3][j])) {

				return src->gameBoard[i][j];
			}
		}
	}

	//horizonal check
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			if (src->gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY
					&& (src->gameBoard[i][j] == src->gameBoard[i][j + 1])
					&& (src->gameBoard[i][j + 1] == src->gameBoard[i][j + 2])
					&& (src->gameBoard[i][j + 2] == src->gameBoard[i][j + 3])) {

				return src->gameBoard[i][j];
			}
		}
	}

	//diagonal check - ascending
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS - 3; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			if (src->gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY
					&& (src->gameBoard[i][j] == src->gameBoard[i + 1][j + 1])
					&& (src->gameBoard[i + 1][j + 1]
							== src->gameBoard[i + 2][j + 2])
					&& (src->gameBoard[i + 2][j + 2]
							== src->gameBoard[i + 3][j + 3])) {

				return src->gameBoard[i][j];
			}
		}
	}
	//diagonal check - descending
	for (int i = 3; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			if (src->gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY
					&& (src->gameBoard[i][j] == src->gameBoard[i - 1][j + 1])
					&& (src->gameBoard[i - 1][j + 1]
							== src->gameBoard[i - 2][j + 2])
					&& (src->gameBoard[i - 2][j + 2]
							== src->gameBoard[i - 3][j + 3])) {

				return src->gameBoard[i][j];
			}
		}
	}

	if (spFiarCheckOver(src))
		return SP_FIAR_GAME_TIE_SYMBOL;

	return '\0';
}

bool spFiarCheckOver(SPFiarGame* src) {
	if (!src)
		return false;

	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		if (src->tops[i] < SP_FIAR_GAME_N_ROWS)
			return false;
	}
	return true;
}

void changePlayer(SPFiarGame* src) {
	if (!src)
		return;

	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	else
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
}

