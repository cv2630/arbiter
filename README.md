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

[fill in later]

## Future Work

[fill in later]