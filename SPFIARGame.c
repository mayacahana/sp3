/*
 * SPFIARGame.c
 *
 *  Created on: 28 במאי 2017
 *      Author: uri
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
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
	if (!res)
		return NULL;

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
	res->movesPlayer1 = spArrayListCreate(historySize);
	res->movesPlayer2 = spArrayListCreate(historySize);

	if (!res->movesPlayer1 || !res->movesPlayer2)
		return NULL;

	return res;
}

SPFiarGame* spFiarGameCopy(SPFiarGame* src) {
	if (!src)
		return NULL;

	SPFiarGame* res = (SPFiarGame*) malloc(sizeof(SPFiarGame));
	if (!res)
		return NULL;

	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++)
			res->gameBoard[i][j] = src->gameBoard[i][j];
	}

	//copy tops
	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
		res->tops[i] = src->tops[i];

	//copy currentPlayer
	res->currentPlayer = src->currentPlayer;

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
	if (!src)
		return;

	spArrayListDestroy(src->movesPlayer1);
	spArrayListDestroy(src->movesPlayer2);
	//free(src->tops);
	//free(src->gameBoard);
	free(src);

	return;
}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col) {
	if (!src || col < 0 || col > SP_FIAR_GAME_N_COLUMNS - 1)
		return SP_FIAR_GAME_INVALID_ARGUMENT;

	if (src->tops[col] >= SP_FIAR_GAME_N_ROWS)
		return SP_FIAR_GAME_INVALID_MOVE;

	src->tops[col] = src->tops[col] + 1;
	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		int msg = spArrayListAddFirst(src->movesPlayer1, col);
		if (msg == SP_ARRAY_LIST_FULL) {
			// the list is full, make room by removing last elem
			spArrayListRemoveLast(src->movesPlayer1);
			spArrayListAddFirst(src->movesPlayer1, col);
		}
		//TODO: change turn ?
	} else { //currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL
		int msg = spArrayListAddFirst(src->movesPlayer2, col);
		if (msg == SP_ARRAY_LIST_FULL) {
			// the list is full, make room by removing last elem
			spArrayListRemoveLast(src->movesPlayer2);
			spArrayListAddFirst(src->movesPlayer2, col);
		}
		// TODO: change turn ?
	}
	src->gameBoard[src->tops[col] - 1][col] = src->currentPlayer;

	return SP_FIAR_GAME_SUCCESS;
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col) {
	if (!src || col < 0|| col > SP_FIAR_GAME_N_COLUMNS - 1 ||
	src->tops[col] >= SP_FIAR_GAME_N_ROWS)
		return false;
	return true;
}

SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src) {
	if (!src)
		return SP_FIAR_GAME_INVALID_ARGUMENT;

	if ((src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL
			&& spArrayListIsEmpty(src->movesPlayer1))
			|| (src->currentPlayer == SP_FIAR_GAME_PLAYER_2_SYMBOL
					&& spArrayListIsEmpty(src->movesPlayer2))) {
		return SP_FIAR_GAME_NO_HISTORY;
	}
	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) {

		int col = spArrayListGetFirst(src->movesPlayer1);
		//int col = spArrayListRemoveLast(src->movesPlayer1);
		spArrayListRemoveFirst(src->movesPlayer1);
		//spArrayListRemoveLast(src->movesPlayer1);
		src->tops[col] = src->tops[col] - 1;
		src->gameBoard[src->tops[col]][col] = SP_FIAR_GAME_EMPTY_ENTRY;
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	} else {
		int col = spArrayListGetFirst(src->movesPlayer2);
		spArrayListRemoveFirst(src->movesPlayer2);
		src->tops[col] = src->tops[col] - 1;
		;
		src->gameBoard[src->tops[col]][col] = SP_FIAR_GAME_EMPTY_ENTRY;
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;

	}
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
	if (!src)
		SP_FIAR_GAME_EMPTY_ENTRY;

	return src->currentPlayer;

}

char spFiarCheckWinner(SPFiarGame* src) {
	if (!src)
		return '\0';

	//vertical check
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS - 3; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			if ((src->gameBoard[i][j] == src->gameBoard[i + 1][j]
					== src->gameBoard[i + 2][j] == src->gameBoard[i + 3][j])
					&& src->gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY)
				return src->gameBoard[i][j]; //return the right symbol according to the winner
		}
	}

	//horizonal check
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			if ((src->gameBoard[i][j] == src->gameBoard[i][j + 1]
					== src->gameBoard[i][j + 2] == src->gameBoard[i][j + 3])
					&& src->gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY)
				return src->gameBoard[i][j]; //return the right symbol according to the winner
		}
	}

	//diagonal check - ascending
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS - 3; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			if ((src->gameBoard[i][j] == src->gameBoard[i + 1][j + 1]
					== src->gameBoard[i + 2][j + 2]
					== src->gameBoard[i + 3][j + 3])
					&& src->gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY)
				return src->gameBoard[i][j]; //return the right symbol according to the winner
		}
	}
	//diagonal check - descending
	for (int i = 3; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS - 3; j++) {
			if ((src->gameBoard[i][j] == src->gameBoard[i - 1][j + 1]
					== src->gameBoard[i - 2][j + 2]
					== src->gameBoard[i - 3][j + 3])
					&& src->gameBoard[i][j] != SP_FIAR_GAME_EMPTY_ENTRY)
				return src->gameBoard[i][j]; //return the right symbol according to the winner
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

void changePlayer(SPFiarGame* src){
	if(!src)
		return;

	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	else
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
}


