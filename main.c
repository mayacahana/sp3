/*
 * main.c
 *
 *  Created on: 5 ביוני 2017
 *      Author: uri
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SPMiniMax.h"
#include "SPFIARParser.h"
#include "SPMainAux.h"
#define historySize 10

int main() {

	setvbuf(stdout, NULL, _IONBF, 0);
	int difficulty = initGame(), cmd = 0;
	if (difficulty == -1) // the user entered quit during init
		exit(0);
	bool winflag = false;
	char winner = ' ';
	SPFiarGame* game = spFiarGameCreate(historySize);
	while (true) {
		//check
		if (!winflag && cmd != NO_CHANGED) {
			spFiarGamePrintBoard(game);
			printf("Please make the next move:\n");
		}
		cmd = playturn(game, difficulty, winflag);
		if (cmd == QUIT)
			//exit(0);
			return 0;

		if (cmd == RESTART) {
			spFiarGameDestroy(game);
			//new game starts
			difficulty = initGame();
			if (difficulty == -1) // the user entered quit during init
				//exit(0);
				return 0;
			game = spFiarGameCreate(historySize);
			winflag = false;
			continue;
		}

		if (cmd == ADD_DISC) {
			winflag = false;
			winner = spFiarCheckWinner(game);
			if (winner != '\0') {
				winnerDeclaration(winner);
				winflag = true;
				continue;
			}
			compPlay(game, difficulty);
			winner = spFiarCheckWinner(game);
			if (winner != '\0') {
				spFiarGamePrintBoard(game);
				winnerDeclaration(winner);
				winflag = true;
				continue;
			}
		}
		if (cmd == NO_CHANGED)
			continue;
		if (cmd == UNDO_MOVE) {
			winflag = false;
			continue;
		}
	}
}


