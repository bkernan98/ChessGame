#include <ctype.h>     // For checking character case (e.g., isupper, islower)
#include <stdlib.h>    // For absolute value function abs()
#include "move_validator.h"
#include "board.h"
#include "check.h"

bool is_path_clear(Move move)
{
    // Check if the path between from and to squares is clear
    int row_step = (move.to_row - move.from_row) ? (move.to_row - move.from_row) / abs(move.to_row - move.from_row) : 0;
    int col_step = (move.to_col - move.from_col) ? (move.to_col - move.from_col) / abs(move.to_col - move.from_col) : 0;

    int current_row = move.from_row + row_step;
    int current_col = move.from_col + col_step;

    while (current_row != move.to_row || current_col != move.to_col) {
        if (board_get(current_row, current_col) != '.') {
            return false; // Path is blocked by another piece
        }
        current_row += row_step;
        current_col += col_step;
    }

    return true; // Path is clear
}
// Helper: check if a move is in a straight line (used for Rook and Queen)
bool is_straight(Move m) {
    return m.from_row == m.to_row || m.from_col == m.to_col;
}

// Helper: check if a move is perfectly diagonal (used for Bishop and Queen)
bool is_diagonal(Move m) {
    return abs(m.from_row - m.to_row) == abs(m.from_col - m.to_col);
}

// Helper: check if a move is in an L-shape (used for Knight)
bool is_knight_move(Move m) {
    int dx = abs(m.from_row - m.to_row);
    int dy = abs(m.from_col - m.to_col);
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

bool is_square_under_attack(int row, int col, char player) {
    char opponent = (player == 'W') ? 'B' : 'W';

    // Iterate through all squares on the board
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char piece = board_get(r, c);

            // Skip empty squares and pieces that don't belong to the opponent
            if (piece == '.' || (player == 'W' && isupper(piece)) || (player == 'B' && islower(piece))) {
                continue;
            }

            // Check if the opponent's piece can attack the target square
            switch (tolower(piece)) {
                    case 'p': { // Pawn attack logic
                        int direction = (opponent == 'W') ? -1 : 1;
                        if (row == r + direction && abs(col - c) == 1) {
                            return true; // Pawn attacks diagonally
                        }
                        break;
                    }
                    case 'r': // Rook attack logic
                        if (is_straight((Move){.from_row = r, .from_col = c, .to_row = row, .to_col = col}) && is_path_clear((Move){.from_row = r, .from_col = c, .to_row = row, .to_col = col})) {
                            return true;
                        }
                        break;
                    case 'b': // Bishop attack logic
                        if (is_diagonal((Move){.from_row = r, .from_col = c, .to_row = row, .to_col = col}) && is_path_clear((Move){.from_row = r, .from_col = c, .to_row = row, .to_col = col})) {
                            return true;
                        }
                        break;
                    case 'q': // Queen attack logic
                        if ((is_straight((Move){.from_row = r, .from_col = c, .to_row = row, .to_col = col}) || 
                             is_diagonal((Move){.from_row = r, .from_col = c, .to_row = row, .to_col = col})) && 
                             is_path_clear((Move){.from_row = r, .from_col = c, .to_row = row, .to_col = col})) {
                            return true;
                        }
                        break;
                    case 'k': { // King attack logic
                        int dx = abs(r - row);
                        int dy = abs(c - col);
                        if (dx <= 1 && dy <= 1) {
                            return true; // King attacks one square in any direction
                        }
                        break;
                    }
                    case 'n': { // Knight attack logic
                        int dx = abs(r - row);
                        int dy = abs(c - col);
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

    return false; // The square is not under attack
}

bool does_move_resolve_check(Move move, char player) {
    char king = (player == 'W') ? WHITE_KING : BLACK_KING;
    int king_row = -1, king_col = -1;

    // Find the king's position
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board_get(row, col) == king) {
                king_row = row;
                king_col = col;
                break;
            }
        }
        if (king_row != -1) break;
    }

    // Simulate the move
    char captured_piece = board_get(move.to_row, move.to_col);
    board_set(move.to_row, move.to_col, board_get(move.from_row, move.from_col));
    board_set(move.from_row, move.from_col, '.');

    // Check if the king is still in check after the move
    bool still_in_check = is_square_under_attack(king_row, king_col, player);

    // Undo the move
    board_set(move.from_row, move.from_col, board_get(move.to_row, move.to_col));
    board_set(move.to_row, move.to_col, captured_piece);

    // If the move captures an attacking piece or blocks the check
    if (!still_in_check) {
        return true; // Move resolves the check
    }

    return false; // Move does not resolve the check
}


// Main function to determine if a move is valid based on the piece and current player
bool is_valid_move(Move move, char player) {
    // Get the piece at the source square
    char piece = board_get(move.from_row, move.from_col);
    // Get the piece at the target square
    char target = board_get(move.to_row, move.to_col);

    // Check if the piece belongs to the current player
    if ((player == 'W' && !isupper(piece)) || (player == 'B' && !islower(piece))) {
        return false; // Not your piece!
    }

    // Prevent capturing your own piece
    if ((player == 'W' && isupper(target)) || (player == 'B' && islower(target))) {
        return false; // Can't capture your own teammate!
    }

    // KING SPECIAL CASE: Always allow the king to move to a safe square, even when in check
    if (tolower(piece) == 'k') {
        int dx = abs(move.from_row - move.to_row);
        int dy = abs(move.from_col - move.to_col);
        if (dx > 1 || dy > 1) return false; // King moves only 1 square
        //simulate the move
        char simulated_piece = board_get(move.to_row, move.to_col);
        board_set(move.to_row, move.to_col, piece);
        board_set(move.from_row, move.from_col, '.');

        bool is_in_check = is_square_under_attack(move.to_row, move.to_col, player);
        // Undo the simulated move
        board_set(move.from_row, move.from_col, piece);
        board_set(move.to_row, move.to_col, simulated_piece);
        
        if (is_in_check) return false; // Can't move into check
        return true;
    }

    // Check if the king is in check
    if (is_in_check(player)) {
        // Ensure the move resolves the check
        if (!does_move_resolve_check(move, player)) {
            return false; // Move does not resolve the check
        }
    }

    // Convert the piece to lowercase to handle both black and white the same way
    switch (tolower(piece)) {
        case 'p': { // Pawn logic
            int direction = (player == 'W') ? -1 : 1;     // White moves up (-1), black moves down (+1)
            int start_row = (player == 'W') ? 6 : 1;      // Starting row for 2-step move

            // Normal 1-step forward move
            if (move.to_col == move.from_col && target == '.') {
                if (move.to_row == move.from_row + direction) return true;

                // First move 2-step forward
                if (move.from_row == start_row && move.to_row == move.from_row + 2 * direction)
                    return board_get(move.from_row + direction, move.from_col) == '.'; // Make sure path is clear
            }

            // Diagonal capture
            if (abs(move.to_col - move.from_col) == 1 && move.to_row == move.from_row + direction && target != '.') {
                return true;
            }

            return false; // Invalid pawn move
        }

            case 'r': // Rook
                return is_straight(move) && is_path_clear(move);

            case 'b': // Bishop
                return is_diagonal(move) && is_path_clear(move);

            case 'q': // Queen
                return (is_straight(move) || is_diagonal(move)) && is_path_clear(move);

            case 'k': { // King: can move only 1 square in any direction
                int dx = abs(move.from_row - move.to_row);
                int dy = abs(move.from_col - move.to_col);
                if (dx > 1 || dy > 1) {
                    return false; // Invalid king move
                }

                // Check if the target square is under attack
                if (is_square_under_attack(move.to_row, move.to_col, player)) {
                    return false; // King cannot move into check
                }

                return true; // Valid king move
            }

            case 'n': // Knight
                return is_knight_move(move);

            default:
                return false; // Unrecognized piece
        }
}