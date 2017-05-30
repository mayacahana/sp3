/*
 * SPArrayList.c
 *
 *  Created on: 28 במאי 2017
 *      Author: uri and maya
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "SPArrayList.h"

SPArrayList* spArrayListCreate(int maxSize) {
	if (maxSize <= 0)
		return NULL;

	SPArrayList* res = (SPArrayList*) malloc(sizeof(SPArrayList));
	if (!res)
		return NULL;

	res->elements = malloc(sizeof(int)*maxSize);;
	if (res->elements == NULL) {
		free(res);
		return NULL;
	}

	res->maxSize = maxSize;
	res->actualSize = 0;

	return res;
}

SPArrayList* spArrayListCopy(SPArrayList* src) {
	if (!src)
		return NULL;

	SPArrayList* copy = (SPArrayList*) malloc(sizeof(SPArrayList));
	if (!copy)
		return NULL;

	copy->maxSize = src->maxSize;
	copy->actualSize = src->actualSize;

	copy->elements = malloc(sizeof(int) * src->maxSize);
	if(copy->elements == NULL) {
		free(copy);
		return NULL;
	}

	for (int i = 0; i < copy->actualSize; i++)
		copy->elements[i] = src->elements[i];

	return copy;
}

void spArrayListDestroy(SPArrayList* src) {
	if (!src)
		return;

	free(src->elements);
	free(src);
	return;
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src) {
	if (!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	src->actualSize = 0;
	//maxSize not affected
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index) {
	if (!src || index < 0 || index > src->actualSize - 1)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	if (src->actualSize + 1 >= src->maxSize)
		return SP_ARRAY_LIST_FULL;

	//shift elements to the right, to make space for the new elem
	for (int i = src->actualSize + 1; i > index; i--)
		src->elements[i] = src->elements[i - 1];

	src->elements[index] = elem;
	src->actualSize = src->actualSize + 1;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem) {
	return spArrayListAddAt(src, elem, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem) {
	return spArrayListAddAt(src, elem, src->actualSize - 1);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index) {
	if (!src || index < 0 || index > src->actualSize - 1)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;

	if (src->actualSize == 0)
		return SP_ARRAY_LIST_FULL;

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
	if (!src || index < 0 || index > src->actualSize - 1)
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
	if (!src || src->actualSize != src->maxSize)
		return false;

	return src->actualSize == src->maxSize;
}

bool spArrayListIsEmpty(SPArrayList* src) {
	if (!src || src->actualSize != 0)
		return false;

	return src->actualSize == 0;
}

