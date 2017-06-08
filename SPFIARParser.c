/*
 * SPFAIRParser.c
 *
 *  Created on: 23 במאי 2017
 *      Author: uri
 */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "SPFIARParser.h"

bool spParserIsInt(const char* str) {

	if (*str == '-') // handle negative number
		++str;
	if (!*str) // handle empty string
		return false;

	while (*str) {
		if (!isdigit(*str)) // the i-th char is not a valid digit
			return false;
		else
			++str;
	}
	return true;
}

SPCommand spParserPraseLine(const char* str) {

	char *strcopy = (char*) malloc(strlen(str) + 1);
	strcpy(strcopy, str);

	SPCommand res;
	res.cmd = SP_INVALID_LINE;
	res.validArg = false;

	bool seencmd = false, seenadd = false;
	char *word;
	word = strtok(strcopy, " \t\r\n");
	while (word != NULL) {
		if (strcmp(word, "") != 0) {
			if (seencmd == false) {
				seencmd = true;
				int arg = spParserCommand(word);
				if (arg == SP_INVALID_LINE) { // invalid line
					res.cmd = arg;
					res.validArg = false;
					break;
				} else if (arg == SP_ADD_DISC) { //add disk
					seenadd = true;
				} else { //other commands
					res.cmd = arg;
					res.validArg = false;
				}
			} else if (seenadd == true && spParserIsInt(word) == true) {
				int val = atoi(word);
				if (val >= 0) {
					res.cmd = SP_ADD_DISC;
					res.validArg = true;
					res.arg = val;
				} else {
					res.cmd = SP_INVALID_LINE;
					res.validArg = false;
					break;
				}
			} else {
				res.cmd = SP_INVALID_LINE;
				res.validArg = false;
				break;
			}
		}
		word = strtok(NULL, " \t\r\n");
	}
	//free memory
	return res;
}

int spParserCommand(char* str) {

	//create a copy
	char *strcopy = (char*) malloc(strlen(str) + 1);
	strcpy(strcopy, str);

	if (strcmp(strcopy, "undo_move") == 0)
		return SP_UNDO_MOVE;
	if (strcmp(strcopy, "add_disc") == 0)
		return SP_ADD_DISC;
	if (strcmp(strcopy, "suggest_move") == 0)
		return SP_SUGGEST_MOVE;
	if (strcmp(strcopy, "quit") == 0)
		return SP_QUIT;
	if (strcmp(strcopy, "restart") == 0)
		return SP_RESTART;

	//free memory
	return SP_INVALID_LINE;

}

