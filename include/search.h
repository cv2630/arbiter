#pragma once
#include "chess.hpp"

struct SearchLimits {
    int depth = 0;
    int movetime = 0; //ms, 0 equals not set
    int wtime = 0, btime = 0, winc = 0, binc = 0; //ms
    bool infinite = false;
};

chess::Move findBestMove(chess::Board& board, const SearchLimits& limits);