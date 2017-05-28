/*
 * SPFIARGame.c
 *
 *  Created on: 28 במאי 2017
 *      Author: uri
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "SPArrayList.h"

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
}

SPFiarGame* spFiarGameCopy(SPFiarGame* src) {
	if (!src)
		return NULL;

	SPFiarGame* res = (SPFiarGame*) malloc(sizeof(SPFiarGame));
	if (!res)
		return NULL;

	//copy gameboard
	char gameboard[SP_FIAR_GAME_N_ROWS][SP_FIAR_GAME_N_COLUMNS];

	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++)
			gameboard[i][j] = src->gameBoard[i][j];

	}
	res->gameBoard = gameboard;
	if (res->gameBoard == NULL) {
		free(res);
		return NULL;
	}

	//copy tops
	int tops[SP_FIAR_GAME_N_COLUMNS];
	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
		tops[i] = src->tops[i];

	res->tops = tops;
	if (res->tops == NULL) {
		free(res->gameBoard);
		free(res);
		return NULL;
	}

	//copy currentPlayer
	res->currentPlayer = src->currentPlayer;
	if (res->currentPlayer == NULL) {
		free(res->gameBoard);
		free(res->tops);
		free(res);
		return NULL;
	}

	//copy movesPlayer
	res->movesPlayer1 = spArrayListCopy(src->movesPlayer1);
	if (res->movesPlayer1 == NULL) {
		free(res->gameBoard);
		free(res->tops);
		free(res->currentPlayer);
		free(res);
		return NULL;
	}
	res->movesPlayer2 = spArrayListCopy(src->movesPlayer2);
	if (res->movesPlayer2 == NULL) {
		free(res->gameBoard);
		free(res->tops);
		free(res->currentPlayer);
		free(res);
		return NULL;
	}
	return res;

}

void spFiarGameDestroy(SPFiarGame* src) {
	if (!src)
		return;

	free(src->movesPlayer1);
	free(src->movesPlayer2);
	free(src->currentPlayer);
	free(src->tops);
	free(src->gameBoard);
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
		spArrayListAddLast(src->movesPlayer1, col);
	} else
		//currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL
		spArrayListAddLast(src->movesPlayer2, col);

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
	//TODO:check if we exceeded historysize

	if ((src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL
			&& spArrayListIsEmpty(src->movesPlayer1))
			|| (src->currentPlayer == SP_FIAR_GAME_PLAYER_2_SYMBOL
					&& spArrayListIsEmpty(src->movesPlayer2)))
		return SP_FIAR_GAME_NO_HISTORY;

	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		int col = spArrayListGetLast(src->movesPlayer1);
		spArrayListRemoveLast(src->movesPlayer1);
		src->tops[col] = src->tops[col]--;
		src->gameBoard[src->tops[col]][col] = src->currentPlayer;
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	} else {
		int col = spArrayListGetLast(src->movesPlayer2);
		spArrayListRemoveLast(src->movesPlayer2);
		src->tops[col] = src->tops[col]--;
		src->gameBoard[src->tops[col]][col] = src->currentPlayer;
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;

	}
	return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src) {
	if (!src)
		return SP_FIAR_GAME_INVALID_ARGUMENT;

	//print the board
}

char spFiarGameGetCurrentPlayer(SPFiarGame* src) {
	if (!src)
		SP_FIAR_GAME_EMPTY_ENTRY;

	return src->currentPlayer;

}

char spFiarCheckWinner(SPFiarGame* src) {
	if (!src)
		return NULL;

}

