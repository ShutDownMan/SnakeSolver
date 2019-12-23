#include "./screen.h"
#include "./snake.h"
#include "./structures.h"
#include "init.h"
#include "input.h"
#include "snake.h"
#include "snake_path.h"

#include <unistd.h>
#include <stdlib.h>

#define DELAY 700000

void generateGridGraph(GridGraph *gridGraph, Snake *snake) {
	cleanGraph(gridGraph);

	SnakeSegment *lastNode = snake->head->next;
	SnakeSegment *curNode = lastNode->next;

	while(curNode != snake->head->next) {
		// gridGraph->hamNodes[(lastNode->x - 1)/2][(lastNode->y - 1)/2]->isFree = FALSE;
		createEdge(gridGraph,
			(Position){.x=(lastNode->x - 1)/2, .y=(lastNode->y - 1)/2},
			(Position){.x=(curNode->x - 1)/2, .y=(curNode->y - 1)/2});

		lastNode = curNode;
		curNode = curNode->next;
	}
}

void mainLoop(gameBoard game, Snake *snake, GridGraph *gridGraph)
{
	// FILE *f = fopen("test.txt", "w+");
	// starts the snake by going north to avoid conflicts with the
	// horizontal snake initial positioning
	directions dir = N;
	char snakeHeadQPos;
	char foodQPos;
	char key;
	char path;

	char alive = TRUE;
	while (alive) {
		// dir = userInput(dir);
		snakeHeadQPos = getQPosition(snake->head->x - 1, snake->head->y - 1);
		foodQPos = getQPosition(game.food.x - 1, game.food.y - 1);

		generateGridGraph(gridGraph, snake);
		// scanf("%*d");
		// mvaddch(40, 0, '>');
		// showGraph(gridGraph);

		path = getNextDirection(gridGraph,
			(Position){.x=(snake->head->x - 1)/2, .y=(snake->head->y - 1)/2, .qPosition=snakeHeadQPos},
			(Position){.x=(game.food.x - 1)/2, .y=(game.food.y - 1)/2, .qPosition=foodQPos});
		key = getKeyFromDirection(snakeHeadQPos, path);
		dir = snakeAIInput(key);

		// int aux = getNextDirection(gridGraph, (Position){.x=(snake->head->x-1)/2, .y=(snake->head->y-1)/2},
				// (Position){.x=(game.food.x-1)/2, .y=(game.food.y-1)/2});
		// fprintf(f, "%d\n", aux);
		// printf("\rDIR = %d", dir);
		// printf("KEY = %d\n\r", key);
		// fprintf(f, "path = %d\n", path);

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
				spawnfruit(&game);
				break;
		}
		usleep(DELAY);
	}
}

int main(void)
{
	gameBoard game;

	Snake snake;

	GridGraph *gridGraph;

	init(&game, &snake, &gridGraph);

	mainLoop(game, &snake, gridGraph);

	endwin();

	return 0;
}
