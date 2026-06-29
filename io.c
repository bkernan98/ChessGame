#include "io.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
/**
 * [REQ-XXX-000] Prompts the user for input (e.g., e2 e4), parses it into the Move struct,
 * and returns true if valid.
 *
 * @param move Pointer to a Move structure to store the parsed input
 * @return bool Returns true if input was successfully parsed into a valid move
 */
bool io_get_move(Move* move) {
    char input[10];
    printf("Enter move (e.g., e2 e4): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return false;
    }
    
    // Remove newline if present
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    // Validate input format (e.g., "e2 e4")
    if (strlen(input) != 5 || input[2] != ' ' || 
        !isalpha(input[0]) || !isdigit(input[1]) || 
        !isalpha(input[3]) || !isdigit(input[4])) {
        printf("Invalid input format. Please use format 'e2 e4'.\n");
        return false;
    }
    
    // Parse columns (a-h -> 0-7)
    move->from_col = tolower(input[0]) - 'a';
    move->to_col = tolower(input[3]) - 'a';
    
    // Parse rows (1-8 -> 0-7) (chess notation has row 1 at the bottom)
    move->from_row = 8 - (input[1] - '0');
    move->to_row = 8 - (input[4] - '0');
    
    // Validate ranges
    if (move->from_col < 0 || move->from_col > 7 ||
        move->from_row < 0 || move->from_row > 7 ||
        move->to_col < 0 || move->to_col > 7 ||
        move->to_row < 0 || move->to_row > 7) {
        printf("Invalid board position. Columns must be a-h, rows must be 1-8.\n");
        return false;
    }
    
    return true;
}
