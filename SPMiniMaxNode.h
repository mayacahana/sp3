#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_
#include "SPFIARGame.h"

//Put all decleartions and constants here

typedef struct sp_mini_max_node_t {
	SPFiarGame* game;
	int value;
} SPMiniMaxNode;

//SPMiniMaxNode* spMiniMaxNodeCreate(SPFiarGame* src);

//void spMiniMaxNodeDestroy(SPMiniMaxNode* src);


/**
 * Calculates the board's score for the current player after the last move.
 * Goes over all possible spans of 4 discs on the board. For each span,
 * counts the number of discs it contains for each player.
 * If there’s any +4 or −4 score: then the score is assigned a huge value INT_MAX or INT_MIN
 * In any other case, we’ll calculate the score according to a constant weight vector:
 * {−5,−2,−1,1,2,5}.
 * @param currentGame - The source game
 * @param currentPlayer - The player we calcuate the score for.
 * @return
 * The board's score.
 *
 */
int scoringFunc(SPFiarGame* currentGame, char currentPlayer);
/**
 * This function helps calculate the board's score. If there is the symbol of
 * the current player - it return 1.
 * if there is an empty entry - return 0.
 * if there is a sympol of the opposite player - return -1.
 * @param currentPlayer - the symbol of the current player.
 * @param symbol - the symbol in the specific entry.
 * @return
 * the numeric value according to above.
 *
 */
int getNumericValue(char currentPlayer, char symbol);
/**
 *
 */
int computeValueRec(SPFiarGame* src, int maxRecLvl, bool flag, char currentPlayer);
int decider(int value, int curr, bool flag);


#endif
