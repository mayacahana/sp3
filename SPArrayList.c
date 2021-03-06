/*
 * SPArrayList.c
 *
 *  Created on: 28 במאי 2017
 *      Author: uri and maya
 */

#include <stdio.h>
//#include <string.h>
//#include <stdbool.h>
#include <stdlib.h>
#include "SPArrayList.h"

SPArrayList* spArrayListCreate(int maxSize) {
	if (maxSize <= 0)
		return NULL;

	SPArrayList* res = (SPArrayList*) malloc(sizeof(SPArrayList));
	if (!res) {
		printf("Error: spArrayListCreate has failed\n");
		exit(1);
	}

	res->elements = (int*) malloc(sizeof(int)*maxSize);
	if (!(res->elements)) {
		printf("Error: spArrayListCreate has failed\n");
		free(res);
		exit(1);
	}

	res->maxSize = maxSize;
	res->actualSize = 0;

	return res;
}

SPArrayList* spArrayListCopy(SPArrayList* src) {
	if (!src)
		return NULL;
	SPArrayList* copy = spArrayListCreate(src->maxSize);

	if (!copy)
		return NULL;

	copy->maxSize = src->maxSize;
	copy->actualSize = src->actualSize;

//	copy->elements = (int*) malloc(sizeof(int) * src->maxSize);
//	if(!copy->elements) {
//		free(copy);
//		return NULL;
//	}

	for (int i = 0; i < copy->actualSize; i++)
		copy->elements[i] = src->elements[i];

	return copy;
}

void spArrayListDestroy(SPArrayList* src) {
	if (!src)
		return;

	free(src->elements);
	free(src);
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src) {
	if (!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	src->actualSize = 0;
	//maxSize not affected and elems not affected
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index) {
	if (!src || index < 0 || index > src->actualSize) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}

	if (src->actualSize == src->maxSize)
		return SP_ARRAY_LIST_FULL;

	//shift elements to the right, to make space for the new elem
	for (int i = src->actualSize; i > index; i--)
		src->elements[i] = src->elements[i - 1];

	src->elements[index] = elem;
	src->actualSize = src->actualSize + 1;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem) {
	return spArrayListAddAt(src, elem, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem) {
	if(!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	return spArrayListAddAt(src, elem, src->actualSize);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index) {
	if (!src || index < 0 || index >= src->actualSize)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	if (src->actualSize == 0)
		return SP_ARRAY_LIST_EMPTY;

	//shift elements to the left, to overwrite the specified elem
	for (int i = index; i < src->actualSize - 1; i++) {
		src->elements[i] = src->elements[i + 1];
	}
	src->actualSize = src->actualSize - 1;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src) {
	return spArrayListRemoveAt(src, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src) {
	if (!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	return spArrayListRemoveAt(src, src->actualSize - 1);
}

int spArrayListGetAt(SPArrayList* src, int index) {
	if (!src || index < 0 || index >= src->actualSize)
		return -1;

	return src->elements[index];
}

int spArrayListGetFirst(SPArrayList* src) {
	return spArrayListGetAt(src, 0);
}

int spArrayListGetLast(SPArrayList* src) {
	if (!src)
		return -1;
	return spArrayListGetAt(src, src->actualSize - 1);
}

int spArrayListMaxCapacity(SPArrayList* src) {
	if (!src)
		return -1;

	return src->maxSize;
}

int spArrayListSize(SPArrayList* src) {
	if (!src)
		return -1;

	return src->actualSize;
}

bool spArrayListIsFull(SPArrayList* src) {
	if (!src || src->actualSize < src->maxSize)
		return false;

	return true;
}

bool spArrayListIsEmpty(SPArrayList* src) {
	if (!src || src->actualSize != 0)
		return false;

	return true;
}

