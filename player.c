// where player.c is and whre the function is defined from player.h
#include "player.h"
#include <stdio.h>
#include <stdlib.h>


// [REQ-XXX-000] Returns the current player.
// char Returns 'W' or 'B' to indicate which player's turn it is.
 
char player_get_current(void){
    return current_player;
}

// [REQ-XXX-000] Toggles the current player between 'W' and 'B'.
void player_switch(void) {
    current_player = (current_player == 'W') ? 'B' : 'W';
}