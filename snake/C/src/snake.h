#ifndef SNAKE_H
#define SNAKE_H

#include "./screen.h"
#include "./structures.h"
#include "input.h"
#include <stdlib.h>

// initiates a new snake segment on the given cords, immediatly adds it to the
// board and screen
SnakeSegment *newSegment(int x, int y, SnakeSegment *next, gameBoard board);

// moves the snake in the indicated direction
void moveSnake(Snake *snake, directions dir, gameBoard board);

// grows the snake by transforming the fruit in a snake segment
void growSnake(Snake *snake, directions dir, gameBoard board);

// checks what happens in the next movement, if it colides, eats or moves
actions checkColision(Snake snake, gameBoard game, directions dir);

#endif
