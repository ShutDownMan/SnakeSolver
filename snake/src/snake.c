#include "./snake.h"

SnakeSegment *newSegment(int x, int y, SnakeSegment *next, gameBoard board)
{
	SnakeSegment *segment = malloc(sizeof(SnakeSegment));
	segment->x = x;
	segment->y = y;
	segment->next = next;
	update(x, y, board, '#');
	return segment;
}

// a small detail here, the snake goes backwards, instead of going
// head -> body -> tails -> head it goes
// head -> tails -> body -> head which possibilitates this movement mechanic
void moveSnake(Snake *snake, directions dir, gameBoard board)
{
	int x = snake->head->x;
	int y = snake->head->y;

	// gets the coordinates of where the next head is going to be
	directionalIncrease(&x, &y, dir);
	update(x, y, board, '#');

	if(!(x == snake->head->next->x && y == snake->head->next->y)) {
		// clear where the tail used to be, since the snake moved away from
		// there
		update(snake->head->next->x, snake->head->next->y, board, ' ');
	}

	// grab the tail of the snake
	snake->head = snake->head->next;

	// give the tail the new head coordinate, effectively making it the new
	// head
	snake->head->x = x;
	snake->head->y = y;
}

// a small detail here, the snake goes backwards, instead of going
// head -> body -> tails -> head it goes
// head -> tails -> body -> head which comes into play on the next assignments
void growSnake(Snake *snake, directions dir, gameBoard board)
{
	int x = snake->head->x;
	int y = snake->head->y;

	// gets the coordinates of where the next head is going to be
	directionalIncrease(&x, &y, dir);

	// makes the current head, which is going to become a body, point at the
	// new head
	snake->head->next = newSegment(x, y, snake->head->next, board);

	// updates the head to the new head
	snake->head = snake->head->next;
}

actions checkColision(Snake snake, gameBoard game, directions dir)
{
	int x = snake.head->x;
	int y = snake.head->y;

	// gets the coordinates of where the next head is going to be
	directionalIncrease(&x, &y, dir);

	switch (game.board[x][y]) {
	case ' ':
		return MOVE;
	case 'o':
		return GROW;
	case '#':
		if(!(x == snake.head->next->x && y == snake.head->next->y)) {
			return DIE;
		}
		return MOVE;
	default:
		return DIE;
	}
}
