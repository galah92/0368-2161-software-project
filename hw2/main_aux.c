#include <stdio.h>
#include "main_aux.h"


int initGame(int heaps[], int *numOfHeaps)
{
    printf("Enter the number of heaps:\n");
    if ((scanf("%d", numOfHeaps) == 1) && (*numOfHeaps < 1 || *numOfHeaps > 32))
    {
        printf("Error: the number of heaps must be between 1 and 32.\n");
        return -1;
    }
    printf("Enter the heap sizes:\n");
    for (int i = 0; i < *numOfHeaps; i++)
    {
        if ((scanf("%d", &heaps[i]) == 1) && heaps[i] <= 0)
        {
            printf("Error: the size of heap %d should be positive.\n", i + 1);
            return -1;
        }
    }
    return 0;
}

int validateInput()
{
    return 0;
}

void printGameStatus(int heaps[], int numOfHeaps, int turnNum)
{
    printf("In turn %d heap sizes are:", turnNum);
    for (int i = 0; i < numOfHeaps; i++)
        printf(" h%d=%d", i + 1, heaps[i]);
    printf(".\n");
}
