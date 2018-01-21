#include <string.h>
#include "FSAStack.h"

#define BYTE unsigned char


struct FSAStack_t {
    BYTE *array;
    size_t dataSize;
	unsigned int capacity;
    unsigned int start;
    unsigned int size;
};


FSAStack* FSAStack_Create(unsigned int capacity, size_t dataSize) {
    FSAStack *stack = malloc(sizeof(FSAStack));
    if (!stack) return NULL;
    stack->dataSize = dataSize;
    stack->capacity = capacity;
    stack->start = stack->size = 0;
    stack->array = malloc(dataSize * capacity);
    if (!stack->array) {
        FSAStack_Destroy(stack);
        return NULL;
    }
    return stack;
}

void FSAStack_Destroy(FSAStack* stack) {
    if (!stack) return;
    if (stack->array) free(stack->array);
    free(stack);
}

int FSAStack_IsFull(const FSAStack* stack) {
    if (!stack) return 0;
    return stack->size == stack->capacity;
}

int FSAStack_IsEmpty(const FSAStack* stack) {
    if (!stack) return 0;
    return stack->size == 0;
}

void FSAStack_Push(FSAStack* stack, void *data) {
    if (!stack) return;
    unsigned int offset = (stack->start + stack->size) % stack->capacity;
    memcpy(stack->array + offset * stack->dataSize, data, stack->dataSize);
    if (FSAStack_IsFull(stack)) {
        stack->start = (stack->start + 1) % stack->capacity;
    } else {
        stack->size++;
    }
}

void* FSAStack_Pop(FSAStack* stack) {
    if (!stack || FSAStack_IsEmpty(stack)) return NULL;
    stack->size--;
    unsigned int offset = (stack->start + stack->size) % stack->capacity;
    return stack->array + offset * stack->dataSize;
}
