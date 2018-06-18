#include <stdio.h>
#include "main_aux.h"
#include "sp_nim.h"
#define MAX_NUM_OF_HEAPS 32


int main()
{
    int heaps[MAX_NUM_OF_HEAPS], numOfHeaps;
    int indexToRemove, toRemove, turnNum = 1;
    if (initGame(heaps, &numOfHeaps) == -1) return 0;
    while (!isGameEnded(heaps, numOfHeaps))
    {
        printGameStatus(heaps, numOfHeaps, turnNum);
        if (turnNum % 2 == 0)
            getUserNextMove(heaps, numOfHeaps, &indexToRemove, &toRemove);
        else
            getComputerNextMove(heaps, numOfHeaps, &indexToRemove, &toRemove);
        heaps[indexToRemove] -= toRemove;
        turnNum++;
    }
    printf(turnNum % 2 == 0 ? "Computer wins!\n" : "You win!\n");
    return 0;
}
