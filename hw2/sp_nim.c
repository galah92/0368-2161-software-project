#include <stdio.h>

int isGameEnded(int heaps[], int numOfHeaps)
{
    for (int i = 0; i < numOfHeaps; i++)
    {
        if (heaps[i])  // we got a non-empty heap
            return 0;
    }
    return 1;
}

void getComputerNextMove(int heaps[], int numOfHeaps, int *index, int *toRemove)
{
    int nimSum = 0;
    *index = 0;
    *toRemove = 1;
    for (int i = 0; i < numOfHeaps; i++)
        nimSum ^= heaps[i];
    if (nimSum)
    {
        while ((heaps[*index] ^ nimSum) >= heaps[*index])
            *index = *index + 1;
        *toRemove = heaps[*index] - (heaps[*index] ^ nimSum);
    }
    else
    {
        while (heaps[*index] <= 0)
            *index = *index + 1;
    }
    printf("Computer takes %d objects from heap %d.\n", *toRemove, *index + 1);
}

void getUserNextMove(int heaps[], int numOfHeaps, int *index, int *toRemove)
{
    printf("Your turn: please enter the heap index and the number of removed objects.\n");
    while ((scanf("%d%d", index, toRemove) != 2) || !(--(*index) >= 0 && *index < numOfHeaps && *toRemove <= heaps[*index] && *toRemove > 0))
        printf("Error: Invalid input.\nPlease enter again the heap index and the number of removed objects.\n");
    printf("You take %d objects from heap %d.\n", *toRemove, *index + 1);
}
