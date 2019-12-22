#include "./board.h"

void newBoard(gameBoard *newGame)
{
	// pretty standard matrix memory allocation
	newGame->board = malloc(sizeof(char *) * newGame->rowSize);
	for (int i = 0; i < newGame->rowSize; i++) {
		newGame->board[i] = malloc(sizeof(char) * newGame->columnSize);
		for (int j = 0; j < newGame->columnSize; j++) {
			// initializes all spaces as empty spaces
			newGame->board[i][j] = ' ';
		}
		// draw borders
		update(i, 0, *newGame, '*');
		update(i, newGame->columnSize - 1, *newGame, '*');
	}
	// more borders
	for (int j = 0; j < newGame->columnSize; j++) {
		update(0, j, *newGame, '*');
		update(newGame->rowSize - 1, j, *newGame, '*');
	}
}

void spawnfruit(gameBoard game)
{
	int x, y;
	char sucess = 0;
	while (!sucess) {
		x = rand() % game.rowSize;
		y = rand() % game.columnSize;
		if (game.board[x][y] == ' ') {
			update(x, y, game, 'o');
			sucess = true;
		}
	}
}
