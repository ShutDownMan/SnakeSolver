#include "./init.h"

// a small detail here, the snake goes backwards, instead of going
// head -> body -> tails -> head it goes
// head -> tails -> body -> head
void initSnake(Snake *snake, gameBoard board)
{
	int middleX = board.rowSize / 2;
	int middleY = board.columnSize / 2;

	snake->head = newSegment(3, 5, NULL, board);

	// so after the head element is created, we create the one that points
	// at it, which is the body of the snake
	SnakeSegment *body =
	    newSegment(2, 5, snake->head, board);

	// once the body is created, we can create the tail element, with the
	// head pointing to it, and it pointing at the body
	snake->head->next = newSegment(1, 5, body, board);

	// createEdge(gridGraph, (Position){.x=middleX, .y=middleY}, (Position){.x=middleX+1, .y=middleY});
	// createEdge(gridGraph, (Position){.x=middleX+1, .y=middleY}, (Position){.x=middleX+2, .y=middleY});
}

void initScreen()
{
	// ncurses screen creation
	initscr();
	noecho();
	// sets the cursor visibility to invisible
	curs_set(FALSE);
}

void init(gameBoard *newGame, Snake *snake, GridGraph **gridGraph)
{
	initScreen();
	srand(time(NULL));

	// gets the current terminal size
	// getmaxyx(stdscr, newGame->columnSize, newGame->rowSize);

	newGame->columnSize = 16;
	newGame->rowSize = 32;

	*gridGraph = createGridGraph((newGame->columnSize-2)/2, (newGame->rowSize-2)/2, 0);
	generatePathInfo(*gridGraph);

	// initializes body, fruit and snake
	newBoard(newGame);
	spawnfruit(newGame);
	initSnake(snake, *newGame);
}
