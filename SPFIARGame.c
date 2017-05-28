/*
 * SPFIARGame.c
 *
 *  Created on: 28 במאי 2017
 *      Author: uri and maya
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
	//TODO sihrior zikaron

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

	res->movesPlayer1 = spArrayListCreate(historySize);
	res->movesPlayer2 = spArrayListCreate(historySize);

}

