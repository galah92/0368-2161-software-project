#ifndef FSASTACK_H_
#define FSASTACK_H_

#include <stdlib.h>
#include <stddef.h>


typedef struct FSAStack FSAStack;

/**
 * Create new FSAStack instance.
 * @param   capacity    max capacity for current instance
 * @param   dataSize    data size of each data to be added
 * @return  NULL if malloc failed
 *          FSAStack* instance otherwise
 */
FSAStack* FSAStack_Create(unsigned int capacity, size_t dataSize);

/**
 * Free all resources for a given FSAStack instance.
 * @param   stack       the instance to destroy
 */
void FSAStack_Destroy(FSAStack* stack);

/**
 * Signal if a given FSAStack instance has reached its capacity.
 * @param   stack       FSAStack instance
 * @return  0 if stack == NULL or stack is not full
 *          1 if stack is full
 */
int FSAStack_IsFull(const FSAStack* stack);

/**
 * Signal if a given FSAStack instance has no elements.
 * @param   stack       FSAStack instance
 * @return  0 if stack == NULL or stack is not empty
 *          1 if stack is full
 */
int FSAStack_IsEmpty(const FSAStack* stack);

/**
 * Push a given element to a given FSAStack instance.
 * Make a copy of the given element - no transfer of ownership.
 * Override bottom-most element if the stack is full.
 * Does nothing if stack == NULL.
 * @param   stack       FSAStack instance
 * @params  data        the data to be pushed, of size elementSize
 */
void FSAStack_Push(FSAStack* stack, void *data);

/**
 * Pop the top-most element of the fiven FSAStack instance.
 * No transfer of ownership - returned pointer should be memcpy().
 * @param   stack       FSAStack instance
 * @return  NULL if stack == NULL or stack is empty
 *          top-most element otherwise
 */
void* FSAStack_Pop(FSAStack* stack);


#endif
