#include "./init.h"

// a small detail here, the snake goes backwards, instead of going
// head -> body -> tails -> head it goes
// head -> tails -> body -> head
void initSnake(Snake *snake, gameBoard board)
{
	int middleX = board.rowSize / 2;
	int middleY = board.columnSize / 2;

	snake->head = newSegment(middleX, middleY, NULL, board);

	// so after the head element is created, we create the one that points
	// at it, which is the body of the snake
	SnakeSegment *body =
	    newSegment(middleX + 2, middleY, snake->head, board);

	// once the body is created, we can create the tail element, with the
	// head pointing to it, and it pointing at the body
	snake->head->next = newSegment(middleX + 1, middleY, body, board);
}

void initScreen()
{
	// ncurses screen creation
	initscr();
	noecho();
	// sets the cursor visibility to invisible
	curs_set(FALSE);
}

void init(gameBoard *newGame, Snake *snake)
{
	initScreen();
	srand(time(NULL));

	// gets the current terminal size
	getmaxyx(stdscr, newGame->columnSize, newGame->rowSize);

	// initializes body, fruit and snake
	newBoard(newGame);
	spawnfruit(*newGame);
	initSnake(snake, *newGame);
}
