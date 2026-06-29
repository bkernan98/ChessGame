// move_validator.h stub
#ifndef MOVE_VALIDATOR_H
#define MOVE_VALIDATOR_H

#include <stdbool.h>
#include "io.h"

bool is_path_clear(Move move);
bool is_valid_move(Move move, char player);
bool is_straight(Move m);
bool is_diagonal(Move m);


#endif
