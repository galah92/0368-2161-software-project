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
    ArrayStack *this = malloc(sizeof(ArrayStack));
    if (!this) return NULL;
    this->dataSize = dataSize;
    this->capacity = capacity;
    this->start = this->size = 0;
    this->array = malloc(dataSize * capacity);
    if (!this->array) {
        ArrayStack_Destroy(this);
        return NULL;
    }
    return this;
}

void ArrayStack_Destroy(ArrayStack* this) {
    if (!this) return;
    if (this->array) free(this->array);
    free(this);
}

unsigned int ArrayStack_Capacity(const ArrayStack* stack) {
    return stack->capacity;
}

unsigned int ArrayStack_Size(const ArrayStack* stack) {
    return stack->size;
}

bool ArrayStack_IsFull(const ArrayStack* this) {
    if (!this) return 0;
    return this->size == this->capacity;
}

bool ArrayStack_IsEmpty(const ArrayStack* this) {
    if (!this) return 0;
    return this->size == 0;
}

void ArrayStack_Push(ArrayStack* this, void *data) {
    if (!this) return;
    unsigned int offset = (this->start + this->size) % this->capacity;
    memcpy(this->array + offset * this->dataSize, data, this->dataSize);
    if (ArrayStack_IsFull(this)) {
        this->start = (this->start + 1) % this->capacity;
    } else {
        this->size++;
    }
}

void* ArrayStack_Pop(ArrayStack* this) {
    if (!this || ArrayStack_IsEmpty(this)) return NULL;
    this->size--;
    unsigned int offset = (this->start + this->size) % this->capacity;
    return this->array + offset * this->dataSize;
}
