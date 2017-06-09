/*
 * SPMainAux.c
 *
 *  Created on: 5 ביוני 2017
 *      Author: uri
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPFIARGame.h"
#include "SPFIARParser.h"
#include "SPMainAux.h"
#include "SPMiniMax.h"

int initGame() {

	int difficulty = 0;
	printf("Please enter the difficulty level between [1-7]:\n");
	char* input = (char*) malloc(SP_MAX_LINE_LENGTH * sizeof(char));
	if (!input) {
		printf("Error: initGame has failed\n");
		free(input);
		exit(1);
	}
	if (!fgets(input, SP_MAX_LINE_LENGTH, stdin)) {
		printf("Error: initGame has failed\n");
		free(input);
		exit(1);
	}
	char* delim = " \t\r\n";
	char* word = strtok(input, delim);
	if (strcmp(word, "quit") == 0) {
		printf("Exiting...\n");
		free(input);
		return -1;
	} else if (!spParserIsInt(word)) {
		printf("Error: invalid level (should be between 1 to 7)\n");
		return initGame();
	}
	difficulty = atoi(word);
	if (difficulty < 1 || difficulty > 7) {
		printf("Error: invalid level (should be between 1 to 7)\n");
		return initGame();
	}

	return difficulty;

}

int playturn(SPFiarGame* game, int difficulty, bool winflag) {
	if (!winflag)
		printf("Please make the next move:\n");

	SPCommand spCmd = parseCommand(game);
	if (spCmd.cmd == SP_QUIT) {
		playQuit(game);
		return QUIT;

	} else if (spCmd.cmd == SP_RESTART) {
		printf("Game restarted!\n");
		return RESTART;

	} else if (spCmd.cmd == SP_ADD_DISC && spCmd.validArg) {
		if (winflag) {
			printf("Error: the game is over\n");
			return playturn(game, difficulty, winflag);
		}
		return playAddDisc(game, spCmd);

	} else if (spCmd.cmd == SP_UNDO_MOVE) {

		if (playUndoMove(game) == 0)
			return NO_CHANGED;
		playUndoMove(game);
		return UNDO_MOVE;

	} else if (spCmd.cmd == SP_SUGGEST_MOVE) {

		if (winflag) {
			printf("Error: the game is over\n");
			return playturn(game, difficulty, winflag);
		}
		return playSuggestedMove(difficulty, winflag, game);

	} else {
		printf("Error: invalid command\n");
		return playturn(game, difficulty, winflag);
	}
}

int playAddDisc(SPFiarGame* game, SPCommand spCmd) {
	if (spFiarGameIsValidMove(game, spCmd.arg - 1)) {
		spFiarGameSetMove(game, spCmd.arg - 1);
		return ADD_DISC;
	} else {
		if (spCmd.arg < 1 || spCmd.arg > 7)
			printf("Error: column number must be in range 1-7\n");
		else if (game->tops[spCmd.arg - 1] == SP_FIAR_GAME_N_ROWS)
			printf("Error: column %d is full\n", spCmd.arg);
		return 3;
	}
	return ADD_DISC;
}

void compPlay(SPFiarGame* game, int difficulty) {
	printf("diff: %d\n",difficulty);
	int col = spMinimaxSuggestMove(game, difficulty);
	printf("Computer move: add disc to column %d\n", col + 1);
	spFiarGameSetMove(game, col);
}

void playQuit(SPFiarGame* game) {
	printf("Exiting...\n");
	spFiarGameDestroy(game);
}

int playUndoMove(SPFiarGame* game) {
	return spFiarGameUndoPrevMoveWithPrint(game);
}

int playSuggestedMove(int difficulty, bool winflag, SPFiarGame* game) {
	//SPFiarGame* copy = spFiarGameCopy(game);
	int col = spMinimaxSuggestMove(game, difficulty);
	//spFiarGameDestroy(copy);
	printf("Suggested move: drop a disc to column %d\n", col + 1);
	return playturn(game, difficulty, winflag);
}

SPCommand parseCommand(SPFiarGame* src) {
	char* input = (char*) malloc(SP_MAX_LINE_LENGTH * sizeof(char));
	if (!input) {
		printf("Error: parserCommand has failed\n");
		spFiarGameDestroy(src);
		exit(1);
	}
	SPCommand spCmd;
	if (!fgets(input, SP_MAX_LINE_LENGTH, stdin)) {
		printf("Error: parseCommand has failed\n");
		exit(1);
	}
	spCmd = spParserPraseLine(input);
	while (spCmd.cmd == SP_INVALID_LINE) {
		printf("Error: invalid command\n");
		printf("Please make the next move:\n");
		if (!fgets(input, SP_MAX_LINE_LENGTH, stdin)) {
			printf("Error: parseCommand has failed\n");
			exit(1);
		}
		spCmd = spParserPraseLine(input);
	}
	//free memory
	return spCmd;
}

void winnerDeclaration(char winner) {
	if (winner == SP_FIAR_GAME_PLAYER_1_SYMBOL)
		printf(
				"Game over: you win\nPlease enter ‘quit’ to exit or ‘restart’ to start a new game!\n");
	else if (winner == SP_FIAR_GAME_PLAYER_2_SYMBOL)
		printf(
				"Game over: computer wins\nPlease enter ‘quit’ to exit or ‘restart’ to start a new game!\n");
	else
		printf(
				"Game over: it’s a tie\nPlease enter ‘quit’ to exit or ‘restart’ to start a new game!\n");
}

int spFiarGameUndoPrevMoveWithPrint(SPFiarGame* src) {
	if (!src) {
		printf("Error: cannot undo previous move!\n");
		return 0;
	}
	int col = 0;
	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		if (spArrayListIsEmpty(src->movesPlayer2)) {
			printf("Error: cannot undo previous move!\n");
			return 0;
		}
		col = spArrayListGetFirst(src->movesPlayer2);
		spArrayListRemoveFirst(src->movesPlayer2);
		changePlayer(src);
	} else {
		if (spArrayListIsEmpty(src->movesPlayer1)) {
			printf("Error: cannot undo previous move!\n");
			return 0;
		}
		col = spArrayListGetFirst(src->movesPlayer1);
		spArrayListRemoveFirst(src->movesPlayer1);
		changePlayer(src);
	}
	src->tops[col] = src->tops[col] - 1;
	src->gameBoard[src->tops[col]][col] = SP_FIAR_GAME_EMPTY_ENTRY;

	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_2_SYMBOL)
		printf("Remove disc: remove computer’s disc at column %d\n", col + 1);
	else if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
		printf("Remove disc: remove user’s disc at column %d\n", col + 1);

	return 1;
}

