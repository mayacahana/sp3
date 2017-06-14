#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_
#include "SPFIARGame.h"

//Put all decleartions and constants here

typedef struct sp_mini_max_node_t {
	SPFiarGame* game;
	int value;
} SPMiniMaxNode;

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
 */
int getNumericValue(char currentPlayer, char symbol);

/**
 * the recursive function that compute the value of the root according
 * the MinMax algorithm as shown in class.
 * @param: src - the current game
 * @param: maxRecLvl - respresents the recursion depth level (set by the difficule level of the game)
 * @param: falg - boolean var represents whether to take MAX between elems or MIN.
 * @param: currentplayer - the symbol of the current player/
 *
 * @return
 * a numeric value according to above
 */
int computeValueRec(SPFiarGame* src, int maxRecLvl, bool flag,char currentPlayer);

/**
 * an aux function that takes MAX between elems of flag is ture and MIN o/w.
 * @param: value - the current MAX/MIN to compare with
 * @param: curr - the candidate for MAX/MIN
 * @param: flag - a boolean variable to decide if MAX actions needs to be taken or MIN
 *
 * @return:
 *a numeric value according to the above.
 */
int decider(int value, int curr, bool flag);

#endif
