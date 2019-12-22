#ifndef SCREEN_H
#define SCREEN_H

/*
 * This is pretty simple for the ncurses implementation, but could quickly
 * become more complex for a openGL or possibily Vulkan implementation
 */

#include "./structures.h"
#include <ncurses.h>

// sets the game board location to its new status, add it to the screen and
// immediatly updates the screen
void update(int x, int y, gameBoard game, char c);

#endif
