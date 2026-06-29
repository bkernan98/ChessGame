#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "board.h"
#include "io.h"
#include "move_validator.h"
#include "check.h"
#include "ui/ui.h" // Presumed to handle board display
#include "player.h"

// Main function of the game loop
void game_loop() {
    Move move;
    board_init();  // Initialize the board

    while (!is_checkmate(player_get_current())) {
        ui_draw_board();  // Assume this function is defined in ui/ui.h

        printf("Player %c, enter your move: ", player_get_current());
        if (!io_get_move(&move)) {
            printf("Invalid input format. Try again.\n");
            continue;
        }

        if (is_valid_move(move, player_get_current())) {
            // Update the board
            char piece = board_get(move.from_row, move.from_col);
            board_set(move.to_row, move.to_col, piece);
            board_set(move.from_row, move.from_col, EMPTY_SQUARE);

            // Check if the game is over (simple checkmate detection)
            
            player_switch();

            if (is_checkmate(player_get_current())) {
                ui_draw_board();
                printf("Checkmate! Player %c loses.\n", player_get_current());
                break;
            }
            
             if(is_in_check(player_get_current())) {
                ui_draw_board();
                printf("Check! Player %c is in check.\n", player_get_current());
            }

              // Switch turns
        }
        else {
            printf("Invalid move. Try again.\n");
        }
    }
}
