#ifndef BOARD_H
#define BOARD_H

#include "./screen.h"
#include "./structures.h"
#include <stdlib.h>

// as the name says, initialize a new board, addapting it to the full size of
// the terminal at start time allocates all necessary memory for the whole board
// upfront
void newBoard(gameBoard *newGame);

// spawns a new fruit in a random place in the board
void spawnfruit(gameBoard game);

#endif
