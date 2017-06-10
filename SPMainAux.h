#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//put auxiliary functions and constants used by the main function here.
#define QUIT 0
#define RESTART 1
#define ADD_DISC 2
#define NO_CHANGED 3
#define UNDO_MOVE 4

int initGame();
int playturn(SPFiarGame* game, int difficulty, bool winflag);
int playAddDisc(SPFiarGame* game, SPCommand spCmd);
void compPlay(SPFiarGame* game, int difficulty);
void playQuit(SPFiarGame* game);
int playUndoMove(SPFiarGame* game);
int playSuggestedMove(int difficulty, bool winflag, SPFiarGame* game);
SPCommand parseCommand(SPFiarGame* src);
void winnerDeclaration(char winner);
int spFiarGameUndoPrevMoveWithPrint(SPFiarGame* src);

#endif
