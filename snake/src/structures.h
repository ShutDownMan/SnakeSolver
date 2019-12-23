#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct food {
	int x, y;
} Food;

// the status of the whole board, and its max size
typedef struct gameBoard {
	int rowSize;	// x
	int columnSize; // y
	char **board;
	Food food;
} gameBoard;

// the directions the snake can go, also known as up, right, down, left and NULL
typedef enum directions { N, E, S, W, INVALID } directions;

// the actions the snake can perfom in game
typedef enum actions { MOVE, GROW, DIE } actions;

// should be circular for simplifaction on movement
typedef struct SnakeSegment {
	int x; // row
	int y; // column
	struct SnakeSegment *next;
} SnakeSegment;

// the snake itself, just here to remove too many layers of pointers shenanigans
typedef struct Snake {
	struct SnakeSegment *head;
} Snake;

#endif
