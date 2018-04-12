#ifndef ARRAY_STACK_H_
#define ARRAY_STACK_H_

#include <stddef.h>
#include <stdbool.h>


typedef struct ArrayStack ArrayStack;

/**
 * Create new ArrayStack instance.
 * @param   capacity    max capacity for current instance
 * @param   dataSize    data size of each data to be added
 * @return  NULL if malloc failed
 *          ArrayStack* instance otherwise
 */
ArrayStack* ArrayStack_Create(unsigned int capacity, size_t dataSize);

/**
 * Create a copy of a given ArrayStack instance.
 * @param   stack       the instance to copy
 * @return  NULL if malloc failed
 *          ChessGame* instance otherwise
 */
ArrayStack* ArrayStack_Copy(const ArrayStack *stack);

/**
 * Free all resources for a given ArrayStack instance.
 * @param   stack       the instance to destroy
 */
ArrayStack* ArrayStack_Destroy(ArrayStack* stack);

/**
 * Retrieve a given ArrayStack instance's max capacity.
 * @param   stack       ArrayStack instance
 * @return  the maximum capacity
 */
unsigned int ArrayStack_Capacity(const ArrayStack* stack);

/**
 * Retrieve a given ArrayStack instance's current number of elements.
 * @param   stack       ArrayStack instance
 * @return  the number of current stored elements
 */
unsigned int ArrayStack_Size(const ArrayStack* stack);

/**
 * Signal if a given ArrayStack instance has reached its capacity.
 * @param   stack       ArrayStack instance
 * @return  false if stack == NULL or stack is not full
 *          true if stack is full
 */
bool ArrayStack_IsFull(const ArrayStack* stack);

/**
 * Signal if a given ArrayStack instance has no elements.
 * @param   stack       ArrayStack instance
 * @return  false if stack == NULL or stack is not empty
 *          true if stack is empty
 */
bool ArrayStack_IsEmpty(const ArrayStack* stack);

/**
 * Push a given element to a given ArrayStack instance.
 * Make a copy of the given element - no transfer of ownership.
 * Override bottom-most element if the stack is full.
 * Does nothing if stack == NULL.
 * @param   stack       ArrayStack instance
 * @params  data        the data to be pushed, of size elementSize
 */
void ArrayStack_Push(ArrayStack* stack, void *data);

/**
 * Pop the top-most element of the given ArrayStack instance.
 * No transfer of ownership - returned pointer should be memcpy().
 * @param   stack       ArrayStack instance
 * @return  NULL if stack == NULL or stack is empty
 *          top-most element otherwise
 */
void* ArrayStack_Pop(ArrayStack* stack);

/**
 * Pop the button-most element of the given ArrayStack instance.
 * No transfer of ownership - returned pointer should be memcpy().
 * @param   stack       ArrayStack instance
 * @return  NULL if stack == NULL or stack is empty
 *          top-most element otherwise
 */
void* ArrayStack_PopLeft(ArrayStack* stack);


#endif
