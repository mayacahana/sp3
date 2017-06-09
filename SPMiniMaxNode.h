#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_
#include "SPFIARGame.h"

//Put all decleartions and constants here

typedef struct sp_mini_max_node_t {
	SPFiarGame* game;
	int value;
} SPMiniMaxNode;

//SPMiniMaxNode functions
SPMiniMaxNode* spMiniMaxNodeCreate(SPFiarGame* src);

void spMiniMaxNodeDestroy(SPMiniMaxNode* src);

int scoringFunc(SPFiarGame* currentGame, char currentPlayer);

int getNumericValue(char currentPlayer, char symbol);

int computeValueRec(SPFiarGame* src, int maxRecLvl, bool flag, char currentPlayer);

int decider(int value, int curr, bool flag);




#endif
