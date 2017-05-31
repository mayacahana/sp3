/*
 * SPMiniMaxNode.c
 *
 *  Created on: 30 במאי 2017
 *      Author: uri
 */
#include "SPMiniMaxNode.h"
#include "SPFIARGame.h"

int getNumericSymbol(SPFiarGame* src, int i, int j) {
	if(!src)
		return 0;

	if(src->currentPlayer == src->gameBoard[i][j])
		return 1;
	if(src->gameBoard[i][j] == SP_FIAR_GAME_EMPTY_ENTRY)
		return 0;
	return -1; // src->gameBoard[i][j] is equals to other symbol player
}


