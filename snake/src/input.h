#ifndef INPUT_H
#define INPUT_H

#include "./structures.h"
#include <ncurses.h>

// reimplementation of conio.h kbhit as not to include the whole library
// checks if there is a pressed char on buffer
int kbhit(void);

// parse input and return a direction in case a directional key was pressed
// invalid otherwise
directions eval(char c);

// moves the (x,y) coords according  to the direction parameter
void directionalIncrease(int *x, int *y, directions dir);

// parses the user input and return the direction if its transverse direction
directions userInput(directions dir);

#endif
