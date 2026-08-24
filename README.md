# arbiter

This project seeks to create a rudimentary UCI chess engine, based off of the following [chess-library](https://github.com/Disservin/chess-library) using the negamax algorithm with alpha-beta pruning.

## Setup

To use this engine, clone the repo using 

```git clone git@github.com:cv2630/arbiter.git```

After cloning the repo, run CMake to build using

```mkdir build && cd build && cmake .. && cmake --build .```

Now that the ```build``` folder is complete, run the engine using

```./engine```

Inputs from the user involve long algebraic notation (e.g. e2e4 or g1f3) taking the "from" square and the "to" square as inputs. The user is white by default, and the game continues until either a checkmate/stalemate is reached or the user enters ```quit``` into the terminal.

## Structure

**Move generation:** we delegate move generation to Disservin's `chess.hpp`. The engine need not generate or validate moves itself, but rather bases it off of the library's list of current legal moves list at each node.

**Search:** we use the negamax algorithm, with alpha-beta pruning, to search for possible moves. Negamax simplifies traditional minimax by exploiting the zero-sum nature of chess: at every node, the current player's best score is the negation of the best score their opponent can force in reply. Alpha-beta pruning, meanwhile, tracks an `[alpha, beta]` window representing the range of viable branches in the game tree. That is, once a move is found that's already worse for the opponent than a line they could force elsewhere, the rest of that branch is skipped.

**Evaluation:** we use a static scoring function measured in [centipawns](https://www.chess.com/blog/raync910/average-centipawn-loss-chess-acpl), which is then run at the search's leaf nodes, combining two independent signals:
- **Material**: sum of standard piece values (pawn=100 ... queen=900) for each side.
- **Positional (piece-square tables)**: per-square bonuses/penalties for each piece type (e.g. promoting central knight placement, or 7th rank rook placement).

These are summed (using the formula `material(us) - material(them) + positional(us) - positional(them)`) to produce a score from the current mover's perspective.

**UCI / interactive layer:** we use a thin command-parsing loop (`uci.cpp`) that either speaks the UCI protocol for GUI compatibility (`uciLoop()`) or runs a simple terminal play mode (`playLoop()`) where the user is White and the engine responds automatically as Black. `arbiter.cpp` contains `playLoop()` by default to allow user input as opposed to raw UCI.

## Future Work
- **Learned evaluation (NNUE-style):** replace or augment the hand-crafted material + PST evaluation with a small neural network trained on labeled positions (e.g. Stockfish-generated evaluations or game outcomes from a database like Lichess).
- **Move ordering (MVV-LVA):** moves are currently searched in the order `legalmoves()` returns them, so alpha-beta prunes far less than it could; trying likely-strong moves first (captures ranked by "most valuable victim, least valuable attacker") would cause more cutoffs earlier in the search.
- **Opening book:** play from a small database of known strong opening lines for the first several moves, rather than searching from move one.
- **King Midgame/Endgame Strategy:** `KING_PST`/`KING_ENDGAME_PST` currently exist but aren't wired into `evaluate()` yet; a way to detect midgame vs. endgame needed to choose between them.
- **Timed Matches:** add a clock feature to add time as a constraint, if desired.
- **Varying Difficulty:** allow the user to choose from several different ELOs.