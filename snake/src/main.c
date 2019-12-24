#include "./screen.h"
#include "./snake.h"
#include "./structures.h"
#include "init.h"
#include "input.h"
#include "snake.h"
#include "snake_path.h"

#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define DELAY 7000

void generateGridGraph(GridGraph *gridGraph, Snake *snake) {
	int i, j, k;
	SnakeSegment *lastNode = snake->head->next;
	SnakeSegment *curNode = lastNode->next;
	Position lastPos, curPos, adjNode;

	cleanGraph(gridGraph);

	i = 0;
	while(curNode != snake->head->next) {
		lastPos = (Position){.x=floor((lastNode->x - 1)/2.0), .y=floor((lastNode->y - 1)/2.0)};
		curPos = (Position){.x=floor((curNode->x - 1)/2.0), .y=floor((curNode->y - 1)/2.0)};

		// gridGraph->hamNodes[(lastNode->x - 1)/2][(lastNode->y - 1)/2]->isFree = FALSE;
		// mvaddch(0, 0, '>');
		// mvprintw(30 + i++, 0, "\r[%2d, %2d] => [%2d, %2d]\n", lastNode->x, lastNode->y, curNode->x, curNode->y);
		// mvprintw(30 + i++, 0, "\r(%2d, %2d) => (%2d, %2d)\n", lastPos.x, lastPos.y, curPos.x, curPos.y);

		createEdge(gridGraph, lastPos, curPos);

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
	Position headPos, foodPos;
	char snakeHeadQPos, foodQPos, key, path;

	char alive = TRUE;
	while (alive) {
		// dir = userInput(dir);
		snakeHeadQPos = getQPosition(snake->head->x - 1, snake->head->y - 1);
		foodQPos = getQPosition(game.food.x - 1, game.food.y - 1);

		headPos = (Position){.x=(snake->head->x - 1)/2, .y=(snake->head->y - 1)/2, .qPosition=snakeHeadQPos};
		foodPos = (Position){.x=(game.food.x - 1)/2, .y=(game.food.y - 1)/2, .qPosition=foodQPos};

		generateGridGraph(gridGraph, snake);

		// scanf("%*d");
		move(28, 0);
		showGraph(gridGraph);
		move(0, 0);


		path = getNextDirection(gridGraph, headPos, foodPos);
		key = getKeyFromDirection(snakeHeadQPos, path);
		dir = snakeAIInput(key);

		// mvprintw(32, 0, "\rpat = %2d", path);
		// mvprintw(33, 0, "\rkey = %2d", key);
		// mvprintw(34, 0, "\rdir = %2d", dir);

		// int aux = getNextDirection(gridGraph, (Position){.x=(snake->head->x-1)/2, .y=(snake->head->y-1)/2},
				// (Position){.x=(game.food.x-1)/2, .y=(game.food.y-1)/2});
		// fprintf(f, "%d\n", aux);
		// printf("\rDIR = %d", dir);
		// printf("KEY = %d\n\r", key);
		// fprintf(f, "path = %d\n", path);
		// while(!kbhit()) {
		// 	usleep(DELAY);
		// }
		// dir = userInput(dir);
		// usleep(DELAY);

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
