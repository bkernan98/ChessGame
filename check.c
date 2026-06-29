#include <ctype.h>
#include <stdlib.h>
#include "check.h"
#include "board.h"
#include "move_validator.h"
#include "io.h"

// Checks if the char pieace is an enemy piece based on the current player
// Returns a true or false 
bool is_enemy(char piece, char player) {
    if (player == 'W') {
        return islower(piece);
    } else if (player == 'B') {
        return isupper(piece);
    }
    return false;
}

/* will check if the peice is currently in "check" like in regular chess */
bool is_in_check(char player) {
    char king = (player == 'W') ? WHITE_KING : BLACK_KING;
    int king_row = -1;
    int king_col = -1;

    /* Find the king's position */
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board_get(row, col) == king) {
                king_row = row;
                king_col = col;
                break;
            }
        }
        if (king_row != -1) {
            break; // Break outer loop if king is found
        }
    }
    if (king_row == -1) {
        return false; // King not found, should not happen in a valid game
    }

    /* Check if any enemy piece can attack the king */
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            char piece = board_get(row, col);
            if (is_enemy(piece, player)) {
                switch (tolower(piece)) {
                    case 'p': { // Pawn attack logic
                        int direction = (player == 'W') ? -1 : 1;
                        if (king_row == row + direction && abs(king_col - col) == 1) {
                            return true; // Pawn attacks diagonally
                        }
                        break;
                    }
                    case 'r': // Rook attack logic
                        if (is_straight((Move){.from_row = row, .from_col = col, .to_row = king_row, .to_col = king_col})) {
                            return true;
                        }
                        break;
                    case 'b': // Bishop attack logic
                        if (is_diagonal((Move){.from_row = row, .from_col = col, .to_row = king_row, .to_col = king_col})) {
                            return true;
                        }
                        break;
                    case 'q': // Queen attack logic
                        if (is_straight((Move){.from_row = row, .from_col = col, .to_row = king_row, .to_col = king_col}) ||
                            is_diagonal((Move){.from_row = row, .from_col = col, .to_row = king_row, .to_col = king_col})) {
                            return true;
                        }
                        break;
                    case 'k': { // King attack logic
                        int dx = abs(row - king_row);
                        int dy = abs(col - king_col);
                        if (dx <= 1 && dy <= 1) {
                            return true; // King attacks one square in any direction
                        }
                        break;
                    }
                    case 'n': { // Knight attack logic
                        int dx = abs(row - king_row);
                        int dy = abs(col - king_col);
                        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
                            return true; // Knight attacks in an L-shape
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    return false; // King is not in check
}


bool is_checkmate(char player) {
    if (!is_in_check(player)) {
        return false; // Not in check, so not checkmate
    }

    char king = (player == 'W') ? WHITE_KING : BLACK_KING;
    int king_row = -1;
    int king_col = -1;

    /* Find the king's position */
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board_get(row, col) == king) {
                king_row = row;
                king_col = col;
                break;
            }
        }
        if (king_row != -1) {
            break; // Break outer loop if king is found
        }
    }

    // Check all adjacent squares for escape
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue; // Skip the king's current position
            int new_row = king_row + dx;
            int new_col = king_col + dy;

            if (new_row >= 0 && new_row < BOARD_SIZE && new_col >= 0 && new_col < BOARD_SIZE) {
                char target_piece = board_get(new_row, new_col);
                if (target_piece == EMPTY_SQUARE) {
                    // Simulate the move
                    board_set(new_row, new_col, king);
                    board_set(king_row, king_col, EMPTY_SQUARE);

                    bool still_in_check = is_in_check(player);

                    // Undo the move
                    board_set(king_row, king_col, king);
                    board_set(new_row, new_col, target_piece);

                    if (!still_in_check) {
                        return false; // Found an escape move
                    }
                }
            }
        }
    }


   
    for (int from_row = 0; from_row < BOARD_SIZE; from_row++) {
    for (int from_col = 0; from_col < BOARD_SIZE; from_col++) {
        char piece = board_get(from_row, from_col);
        if (piece == EMPTY_SQUARE || is_enemy(piece, player)) continue;

        for (int to_row = 0; to_row < BOARD_SIZE; to_row++) {
            for (int to_col = 0; to_col < BOARD_SIZE; to_col++) {
                Move move = {from_row, from_col, to_row, to_col};
                if (!is_valid_move(move, player)) continue; // Skip invalid moves
                // Simulate the move
                char captured = board_get(to_row, to_col);
                board_set(to_row, to_col, piece);
                board_set(from_row, from_col, EMPTY_SQUARE);

                bool still_in_check = is_in_check(player);

                // Undo the move
                board_set(from_row, from_col, piece);
                board_set(to_row, to_col, captured);

                if (!still_in_check) {
                    return false; // Found an escape move
                }
            }
        }
    }
}

    // No escape move found
    return true;
}

