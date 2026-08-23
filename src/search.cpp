#include "search.h"
#include "eval.h"
#include <chrono>
#include <iostream>
#include <limits>

using namespace chess;

namespace {

constexpr int infty=99999999; //
constexpr int checkmate=999999;

int negamax(Board& board, int depth) {
//most sources use minimax, switch to negamax for single maximization?

}

} // namespace

Move findBestMove(Board& board, const SearchLimits& limits) {
    Movelist moves;
    movegen::legalmoves(moves, board);
    if(moves.empty()) {
        return Move::NO_MOVE;
    }

    int depth;
    if(limits.depth > 0) {
        depth=limits.depth; } 
    else {
        depth=4;
    }
    Move bestMove= moves[0];
    int bestScore= -infty;


    for(const auto&m:moves) {

        board.makeMove(m);
        int score= -negamax(board, depth - 1);
        board.unmakeMove(m);
        if (score > bestScore) {
            bestScore=score;
            bestMove= m;
        }}
    return bestMove;

}