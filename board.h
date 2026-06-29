// board.h stub
#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZE 8

/* Piece definitions */
// White pieces (uppercase)
#define WHITE_KING    'K'
#define WHITE_QUEEN   'Q'
#define WHITE_ROOK    'R'
#define WHITE_BISHOP  'B'
#define WHITE_KNIGHT  'N'
#define WHITE_PAWN    'P'

// Black pieces (lowercase)
#define BLACK_KING    'k'
#define BLACK_QUEEN   'q'
#define BLACK_ROOK    'r'
#define BLACK_BISHOP  'b'
#define BLACK_KNIGHT  'n'
#define BLACK_PAWN    'p'

// Empty square
#define EMPTY_SQUARE  '.'

extern char board[BOARD_SIZE][BOARD_SIZE];

// Initializes the board to the standard starting position
void board_init();

// Gets the piece at a given position
char board_get(int row, int col);

// Sets the piece at a given position
void board_set(int row, int col, char piece);

#endif
