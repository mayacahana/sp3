/*
 * SPFAIRParser.c
 *
 *  Created on: 23 במאי 2017
 *      Author: uri
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool spParserIsInt(const char* str) {

	if (*str == '-') // handle negative number
		str++;
	if (!*str) // handle empty string
		return false;

	while (*str) {
		if (!isdigit(*str)) // the i-th char is not a valid digit
			return false;
		else
			str++;
	}
	return true;
}

SPCommand spParserPraseLine(char* str) {

	SPCommand* res = malloc(sizeof(SPCommand));
	if(!res)
		return NULL;

	res->cmd = SP_INVALID_LINE;
	res->validArg = false;

	bool seencmd = false, seenadd = false;
	char *word, delim = "\t\r\n";
	word = strtok(str, delim);
	while(word != NULL) {

		if(strcmp(word, "") == 0 || strcmp(word, " ") == 0)
			continue;
		else {
			if(seencmd == false) {
				seencmd = true;
				int arg = spParserCommand(word);
				if(arg == 5)  { // invalid line
					res->cmd = arg;
					res->validArg = false;
					break;
				} else if (arg == 2)  //add disk
					seenadd = true;
				else { //other commands
					res->cmd = arg;
					res->validArg = false;
				}

			}
			else if(seenadd == true && spParserIsInt(word) == true) {
				int val = atoi(word);
				if(val >= 1 && val <= 7) {
					res ->cmd = SP_ADD_DISC;
					res->validArg = true;
					res->arg = val;
				} else {
					res->cmd = SP_INVALID_LINE;
					res->validArg = false;
					break; // (?)
				}
			}
			else {
				res->cmd = SP_INVALID_LINE;
				res->validArg = false;
				break; // (?)
			}
		}
		word = strtok(NULL, delim);
	}
	return res;
}

int spParserCommand(char* str) {

	//create a copy
	char* strcopy = strcpy(strcopy, str);

	//convert to lower case
	for(; *strcopy; ++strcopy) *strcopy = tolower(*strcopy);

	if(strcmp(strcopy, "undo_move") == 0)
		return SP_UNDO_MOVE;
	if(strcmp(strcopy, "add_disk") == 0)
		return SP_ADD_DISC;
	if(strcmp(strcopy, "suggest_move") == 0)
		return SP_SUGGEST_MOVE;
	if(strcmp(strcopy, "quit") == 0)
			return SP_QUIT;
	if(strcmp(strcopy, "restart") == 0)
			return SP_RESTART;

	return SP_INVALID_LINE;

}


