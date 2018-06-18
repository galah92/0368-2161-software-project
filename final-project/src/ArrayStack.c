#include <stdlib.h>
#include <string.h>
#include "ArrayStack.h"

#define BYTE unsigned char


struct ArrayStack {
    BYTE *array;
    size_t elementSize;
	unsigned int capacity;
    unsigned int start;
    unsigned int size;
};

ArrayStack* ArrayStack_Create(unsigned int capacity, size_t elementSize) {
    ArrayStack *stack = malloc(sizeof(ArrayStack));
    if (!stack) return NULL;
    stack->elementSize = elementSize;
    stack->capacity = capacity;
    stack->start = stack->size = 0;
    stack->array = malloc(elementSize * capacity);
    if (!stack->array) ArrayStack_Destroy(stack);
    return stack;
}

ArrayStack* ArrayStack_Copy(const ArrayStack *stack) {
    ArrayStack *copy = malloc(sizeof(ArrayStack));
    memcpy(copy, stack, sizeof(ArrayStack));
    copy->array = malloc(copy->elementSize * copy->capacity);
    if (!copy->array) ArrayStack_Destroy(copy);
    memcpy(copy->array, stack->array, copy->capacity);
    return copy;
}

ArrayStack* ArrayStack_Destroy(ArrayStack *stack) {
    if (!stack) return NULL;
    if (stack->array) free(stack->array);
    free(stack);
    return NULL;
}

unsigned int ArrayStack_Capacity(const ArrayStack *stack) {
    return stack->capacity;
}

unsigned int ArrayStack_Size(const ArrayStack *stack) {
    return stack->size;
}

bool ArrayStack_IsFull(const ArrayStack *stack) {
    if (!stack) return false;
    return stack->size == stack->capacity;
}

bool ArrayStack_IsEmpty(const ArrayStack *stack) {
    if (!stack) return false;
    return stack->size == 0;
}

void ArrayStack_Push(ArrayStack *stack, void *element) {
    if (!stack) return;
    unsigned int offset = (stack->start + stack->size) % stack->capacity;
    memcpy(stack->array + offset * stack->elementSize, element, stack->elementSize);
    if (ArrayStack_IsFull(stack)) {
        stack->start = (stack->start + 1) % stack->capacity;
    } else {
        stack->size++;
    }
}

void* ArrayStack_Pop(ArrayStack *stack) {
    if (!stack || ArrayStack_IsEmpty(stack)) return NULL;
    stack->size--;
    unsigned int offset = (stack->start + stack->size) % stack->capacity;
    return stack->array + offset * stack->elementSize;
}

void* ArrayStack_PopLeft(ArrayStack *stack) {
    if (!stack || ArrayStack_IsEmpty(stack)) return NULL;
    stack->size--;
    void *startElement = stack->array + stack->start * stack->elementSize;
    stack->start = (stack->start + 1) % stack->capacity;
    return startElement;
}
