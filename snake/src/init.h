#ifndef INIT_H
#define INIT_H

#include "./snake.h"
#include "./structures.h"
#include "board.h"
#include <stdio.h>
#include <time.h>

// inits the game, start a screen and set base parameters for board and snake
void init(gameBoard *newGame, Snake *snake);

// starts a new snake, position it roughly at the middle of the board
void initSnake(Snake *snake, gameBoard board);

// executes a series of ncurses calls to initiate a screen
void initScreen();

#endif
