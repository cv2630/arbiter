
#pragma once
#include "chess.hpp"
 
// static evaluation using centipawns
// from the perspective of the side to move
// note to slef, we define positive = good for side to move
int evaluate(const chess::Board& board);
 