#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SPArrayList.h"

#define MIN_INT -2147483648

SPArrayList* spArrayListCreate(int maxSize){
	
	if (maxSize > 0){
		SPArrayList *sp = (SPArrayList *) malloc(sizeof(SPArrayList));
		if (sp == NULL) return NULL; //return NULL if malloc failed
		sp -> elements = (int *) calloc(maxSize , sizeof(int));
		if (sp -> elements == NULL){
			free(sp);
			return NULL; //return NULL if malloc failed
		}
		sp -> maxSize = maxSize;
		sp -> actualSize = 0;
		return sp;
	}

		return NULL; //maxSize <= 0

}

SPArrayList* spArrayListCopy(SPArrayList* src){

	if (src != NULL){
		SPArrayList *sp = (SPArrayList *) malloc(sizeof(SPArrayList));
		if (sp == NULL) return NULL; //return NULL if malloc failed
		sp -> elements = (int *) calloc((src -> maxSize) , sizeof(int)); 
		if (sp -> elements == NULL){
			free(sp);
			return NULL; //return NULL if malloc failed
		}
		sp -> maxSize = src -> maxSize;
		sp -> actualSize = src -> actualSize;
		memcpy(sp -> elements , src -> elements , (src -> actualSize) * sizeof(int));// elements array copy //TO-DO - check copy of 0:actualSize or 0:maxSize
		return sp;
	}

	return NULL; //src == NULL

}

void spArrayListDestroy(SPArrayList* src){
	
	if (src != NULL){
		free(src -> elements);
		free(src);
	}

}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){
	
	if (src != NULL){
		src -> actualSize = 0; 
		return SP_ARRAY_LIST_SUCCESS;
	}

	return SP_ARRAY_LIST_INVALID_ARGUMENT;

}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index){
	
	if (src != NULL && index >= 0 && index < src -> maxSize){ //Addition is legal
		if (src -> actualSize == src -> maxSize) return SP_ARRAY_LIST_FULL; 
		memmove((src -> elements) + index + 1 , (src -> elements) + index , (src -> actualSize) - index); //shift elements
		*((src -> elements) + index) = elem;
		src -> actualSize += 1;
		return SP_ARRAY_LIST_SUCCESS;
	}

	return SP_ARRAY_LIST_INVALID_ARGUMENT;

}

 SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem){

 	return spArrayListAddAt(src , elem , 0);

 }

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem){

	if (src != NULL){ //Addition is legal
		if (src -> actualSize == src -> maxSize) return SP_ARRAY_LIST_FULL;
		*((src -> elements) + (src -> actualSize)) = elem; //add the element at the end
		src -> actualSize += 1;
		return SP_ARRAY_LIST_SUCCESS;
	}

	return SP_ARRAY_LIST_INVALID_ARGUMENT;

}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){

		if (src != NULL && index >= 0 && index < src -> maxSize){ //delete is legal
		if (src -> actualSize == 0) return SP_ARRAY_LIST_EMPTY;
		for (int i = index ; i < (src -> actualSize) - 1 ; i++) *(src -> elements) = *((src -> elements) + 1); //shift elements
		src -> actualSize -= 1;
		return SP_ARRAY_LIST_SUCCESS;
	}

	return SP_ARRAY_LIST_INVALID_ARGUMENT;

}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){

	return spArrayListRemoveAt(src , 0);

}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	
	if (src != NULL){ //delete is legal
		if (src -> actualSize == 0) return SP_ARRAY_LIST_EMPTY;
		src -> actualSize -= 1;
		return SP_ARRAY_LIST_SUCCESS;
	}

	return SP_ARRAY_LIST_INVALID_ARGUMENT;

}

int spArrayListGetAt(SPArrayList* src, int index){

	if (src != NULL && index >= 0 && index < src -> maxSize){ //list not NULL and the size is legal
		return *((src -> elements) + index);
	}
	return MIN_INT;

}

int spArrayListGetFirst(SPArrayList* src){

	return spArrayListGetAt(src , 0);

}

int spArrayListGetLast(SPArrayList* src){

	if (src != NULL) return spArrayListGetAt(src , (src -> actualSize) - 1);
	return MIN_INT;

}

int spArrayListMaxCapacity(SPArrayList* src){

	if (src != NULL) return src -> maxSize;
	return MIN_INT;

}

int spArrayListSize(SPArrayList* src){

	if (src != NULL) return src -> actualSize;
	return MIN_INT;

}

bool spArrayListIsFull(SPArrayList* src){

	if (src != NULL && (src -> actualSize) == (src -> maxSize)) return true;
	return false;

}

bool spArrayListIsEmpty(SPArrayList* src){

	if (src != NULL && (src -> actualSize) == 0) return true;
	return false;

}
