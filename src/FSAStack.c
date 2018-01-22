#include <string.h>
#include "FSAStack.h"

#define BYTE unsigned char


struct FSAStack {
    BYTE *array;
    size_t dataSize;
	unsigned int capacity;
    unsigned int start;
    unsigned int size;
};


FSAStack* FSAStack_Create(unsigned int capacity, size_t dataSize) {
    FSAStack *this = malloc(sizeof(FSAStack));
    if (!this) return NULL;
    this->dataSize = dataSize;
    this->capacity = capacity;
    this->start = this->size = 0;
    this->array = malloc(dataSize * capacity);
    if (!this->array) {
        FSAStack_Destroy(this);
        return NULL;
    }
    return this;
}

void FSAStack_Destroy(FSAStack* this) {
    if (!this) return;
    if (this->array) free(this->array);
    free(this);
}

int FSAStack_IsFull(const FSAStack* this) {
    if (!this) return 0;
    return this->size == this->capacity;
}

int FSAStack_IsEmpty(const FSAStack* this) {
    if (!this) return 0;
    return this->size == 0;
}

void FSAStack_Push(FSAStack* this, void *data) {
    if (!this) return;
    unsigned int offset = (this->start + this->size) % this->capacity;
    memcpy(this->array + offset * this->dataSize, data, this->dataSize);
    if (FSAStack_IsFull(this)) {
        this->start = (this->start + 1) % this->capacity;
    } else {
        this->size++;
    }
}

void* FSAStack_Pop(FSAStack* this) {
    if (!this || FSAStack_IsEmpty(this)) return NULL;
    this->size--;
    unsigned int offset = (this->start + this->size) % this->capacity;
    return this->array + offset * this->dataSize;
}
