
#include <stdio.h>
#include "ui.h"
#include "board.h"

void ui_init() {}

void ui_draw_board() {
    //draw the board using board_get
   printf("  a b c d e f g h\n"); 
    for (int i = 1; i <= 8; i++) {
        printf("%d ", 9-i);
        for (int j = 0; j < 8; j++) {
            char piece = board_get(i - 1, j);
            if (piece == EMPTY_SQUARE) {
                printf(". ");
            } else {
                printf("%c ", piece);
            }
        }
        printf("%d\n", 9-i);
    }
    printf("  a b c d e f g h\n");
    /*printf("  a b c d e f g h\n");
    for (int i = 8; i >= 1; i--) {
        printf("%d ", i);
        for (int j = 0; j < 8; j++) {
            printf(". ");
        }
        printf("%d\n", i);
    }
    printf("  a b c d e f g h\n");
    */
}

void ui_show_message(const char* msg) {
    printf("%s\n", msg);
}

void ui_cleanup() {}
