#include "FSAStack.h"


struct FSAStack_t {
    void **elements;
    size_t elementSize;
	unsigned int capacity;
    unsigned int startIndex;
    unsigned int size;
};


FSAStack* FSAStack_Create(unsigned int capacity, size_t elementSize) {
    FSAStack *stack = malloc(sizeof(FSAStack));
    if (!stack) return NULL;
    stack->elementSize = elementSize;
    stack->capacity = capacity;
    stack->startIndex = stack->size = 0;
    stack->elements = malloc(elementSize * capacity);
    if (!stack->elements) {
        FSAStack_Destroy(stack);
        return NULL;
    }
    return stack;
}

void FSAStack_Destroy(FSAStack* stack) {
    if (!stack) return;
    if (stack->elements) free(stack->elements);
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
    stack->elements[(stack->startIndex + stack->size) % stack->capacity] = data;
    if (FSAStack_IsFull(stack)) {
        stack->startIndex = (stack->startIndex + 1) % stack->capacity;
    } else {
        stack->size++;
    }
}

void* FSAStack_Pop(FSAStack* stack) {
    if (!stack || FSAStack_IsEmpty(stack)) return NULL;
    stack->size--;
    return stack->elements[(stack->startIndex + stack->size) % stack->capacity];
}
