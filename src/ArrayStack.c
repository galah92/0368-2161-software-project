#include <string.h>
#include "ArrayStack.h"

#define BYTE unsigned char


struct ArrayStack {
    BYTE *array;
    size_t dataSize;
	unsigned int capacity;
    unsigned int start;
    unsigned int size;
};

ArrayStack* ArrayStack_Create(unsigned int capacity, size_t dataSize) {
    ArrayStack *stack = malloc(sizeof(ArrayStack));
    if (!stack) return NULL;
    stack->dataSize = dataSize;
    stack->capacity = capacity;
    stack->start = stack->size = 0;
    stack->array = malloc(dataSize * capacity);
    if (!stack->array) {
        ArrayStack_Destroy(stack);
        return NULL;
    }
    return stack;
}

ArrayStack* ArrayStack_Copy(const ArrayStack *stack) {
    ArrayStack *copy = malloc(sizeof(ArrayStack));
    memcpy(copy, stack, sizeof(ArrayStack));
    copy->array = malloc(copy->dataSize * copy->capacity);
    if (!copy->array) {
        ArrayStack_Destroy(copy);
        return NULL;
    }
    memcpy(copy->array, stack->array, copy->capacity);
    return copy;
}

void ArrayStack_Destroy(ArrayStack* stack) {
    if (!stack) return;
    if (stack->array) free(stack->array);
    free(stack);
}

unsigned int ArrayStack_Capacity(const ArrayStack* stack) {
    return stack->capacity;
}

unsigned int ArrayStack_Size(const ArrayStack* stack) {
    return stack->size;
}

bool ArrayStack_IsFull(const ArrayStack* stack) {
    if (!stack) return false;
    return stack->size == stack->capacity;
}

bool ArrayStack_IsEmpty(const ArrayStack* stack) {
    if (!stack) return false;
    return stack->size == 0;
}

void ArrayStack_Push(ArrayStack* stack, void *data) {
    if (!stack) return;
    unsigned int offset = (stack->start + stack->size) % stack->capacity;
    memcpy(stack->array + offset * stack->dataSize, data, stack->dataSize);
    if (ArrayStack_IsFull(stack)) {
        stack->start = (stack->start + 1) % stack->capacity;
    } else {
        stack->size++;
    }
}

void* ArrayStack_Pop(ArrayStack* stack) {
    if (!stack || ArrayStack_IsEmpty(stack)) return NULL;
    stack->size--;
    unsigned int offset = (stack->start + stack->size) % stack->capacity;
    return stack->array + offset * stack->dataSize;
}
