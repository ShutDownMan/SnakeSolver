#include "./screen.h"
#include "./snake.h"
#include "./structures.h"
#include "init.h"
#include "input.h"
#include "snake.h"

#include <unistd.h>

#define DELAY 70000

void mainLoop(gameBoard game, Snake *snake)
{
	// starts the snake by going north to avoid conflicts with the
	// horizontal snake initial positioning
	directions dir = N;

	char alive = TRUE;
	while (alive) {
		dir = userInput(dir);
		switch (checkColision(*snake, game, dir)) {
		case DIE:
			usleep(DELAY);
			alive = false;
			break;
		case MOVE:
			moveSnake(snake, dir, game);
			break;
		case GROW:
			growSnake(snake, dir, game);
			spawnfruit(game);
			break;
		}
		usleep(DELAY);
	}
}

int main(void)
{
	gameBoard game;

	Snake snake;

	init(&game, &snake);

	mainLoop(game, &snake);

	endwin();

	return 0;
}
