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

/**
 * Initializing a new game. This function asks for the difficulty level and create a new
 * game board.
 * @return
 * the difficulty level of the game
 */
int initGame();

/**
 * the function is getting an input from the user and parser the command.
 * @param game- The source game
 * @param difficulty - the difficulty level of the game
 * @param winflag - a boolean variant indicates of there's a winner - True. False otherwise.
 *
 * @return
 * a number that indicates which command should the program commit.
 */
int playturn(SPFiarGame* game, int difficulty, bool winflag);
/**
 * Adding a disc to the specified column. Checks if a disk can be put in this column.
 * If its not possible- an error message appeared.
 *
 * @param game- The source game
 * @param spCmd - the command to parser
 * @return
 * an int value according to step to be performed.
 *
 */
int playAddDisc(SPFiarGame* game, SPCommand spCmd);
/**
 * Play the computer's move. Call the suggestMove function and
 * add the disc to the column number that returned.
 * Print a message says which move the computer committed.
 *
 * @param game - The source game
 * @param difficulty - the difficulty level of the game
 */
void compPlay(SPFiarGame* game, int difficulty);
/**
 * exiting the game. Destroy all the memory. Print a message says the game
 * is exiting.
 *
 * @param game - The source game
 */
void playQuit(SPFiarGame* game);
/**
 * Undo the last move - removes a disc that was put in the previous move and changes the current
 * player's turn. If the user invoked this command more than historySize times
 * in a row then an error occurs.
 *
 * @param game - The source game
 * @return
 * 17 if the command was successful, 0 otherwise.
 */
int playUndoMove(SPFiarGame* game);
/**
 * Play the suggested move offered by the minimax algorithm and print a
 * message.
 * @param difficulty - the difficulty level of the game
 * @param winflag - True iff there is a winner, False otherwise
 * @param game - The source game
 *
 * @return
 * an int value according to step to be performed.
 */
int playSuggestedMove(int difficulty, bool winflag, SPFiarGame* game);
/**
 * Gets an input from the user, parser it and return the command to commit
 *
 * @param src - The source game
 * @param winflag - True iff there is a winner, False otherwise
 *
 * @return
 * the parsed command.
 */
SPCommand parseCommand(SPFiarGame* src);
/**
 * Prints a message informing the winner. If there is a tie -
 * prints a message accordingly.
 *
 * @param winner - the symbol of the winner - 'X' or 'O'
 */
void winnerDeclaration(char winner);
/**
 * Commit the undo-move move and prints a message accordingly.
 * @param src - The source game
 *
 * @return
 * 17 if the command was successful, 0 otherwise.
 */
int spFiarGameUndoPrevMoveWithPrint(SPFiarGame* src);


#endif
