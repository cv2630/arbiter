#include "search.h"
#include "eval.h"
#include <chrono>
#include <iostream>
#include <limits>

using namespace chess;

//add in time?
//lookup for how to record time in terminal

namespace {

constexpr int infty=99999999; //
constexpr int checkmate=999999;
int negamax(Board& board, int depth,int alpha,int beta){
if(depth == 0){
     return evaluate(board);
}

Movelist moves;
movegen::legalmoves(moves, board);
if (moves.empty()) {
    if (board.inCheck()) {
        return -checkmate - depth;
    } 
    else {
return 0;
}}

int best = -infty;
for(const auto& m:moves){
    board.makeMove(m);
    int score= -1*(negamax(board, depth-1, -beta, -alpha)); //check + -
    board.unmakeMove(m);
    if(score>best) {
        best = score;
    }
    if(best>alpha) {
        alpha=best;
    }
    if (alpha>=beta) {
        break;
} //alpha-beta pruning
}

return best;
}} // namespace

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
        int score= -negamax(board, depth - 1, -infty, infty);
        board.unmakeMove(m);
        if (score > bestScore) {
            bestScore=score;
            bestMove= m;
        }}
    return bestMove;

}