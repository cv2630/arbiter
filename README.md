# arbiter

This project seeks to create a rudimentary UCI chess engine, based off of the following [chess-library](https://github.com/Disservin/chess-library).

## Setup

To use this engine, clone the repo using 

```git clone git@github.com:cv2630/arbiter.git```

After cloning the repo, run CMake to build using

```mkdir build && cd build && cmake .. && cmake --build .```

Now that the ```build``` folder is complete, run the engine using

```./engine```

Inputs from the user involve long algebraic notation (e.g. e2e4 or g1f3) taking the "from" square and the "to" square as inputs. The user is white by default, and the game continues until either a checkmate/stalemate is reached or the user enters ```quit``` into the terminal.

## Structure

[]

## Future Work
- **Learned evaluation (NNUE-style):** replace or augment the hand-crafted material + PST evaluation with a small neural network trained ono labeled positions (e.g. Stockfish-generated evaluations or game outcomes from a database like Lichess).
- **Move ordering (MVV-LVA):** moves are currently searched in the order `legalmoves()` retursn them, so alpha-beta prunes far less than ti could; trying likely-strong moves first (captures ranked by "most valuable victim, least valuable attacker") would cause more cutoffs earlier in the search.
- **Opening book:** play from a small database of known strong opening lines
  for the first several moves, rather than searching from move one.
- **King Midgame/Endgame Strategy:** — `KING_PST`/`KING_ENDGAME_PST` currently exist but aren't wired into `evaluate()` yet; a way to detect midgame vs. endgame needed to choose between them.
- **Tined Matches:** add a clock feature to add time as a constraint, if desired.
- **Varying Difficulty:** allow the user to choose from several different ELOs.