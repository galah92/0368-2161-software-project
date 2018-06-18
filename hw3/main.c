#include "SPMainAux.h"
#include "SPFIARGame.h"

/**
 *  the function run the game modules
 *  @return
 *  0 if the user quit the game
 */
int main() {
    int difficulty;
    SPFiarGame *game = NULL;
    GameState state = GAME_STATE_RESTARTED;
    while (state != GAME_STATE_QUIT) { //the user keep playing
        if (state == GAME_STATE_RESTARTED) { 
            state = InitGame(&game, &difficulty); //initialize new game
        } else {
            state = ProcessCommand(game, state, difficulty); //handle the commands
        }
        PrintGameState(game, state);
    }
    return 0;
}
