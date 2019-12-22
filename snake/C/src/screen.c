#include "./screen.h"

void update(int x, int y, gameBoard game, char c)
{
	game.board[x][y] = c;
	mvaddch(y, x, game.board[x][y]);
	refresh();
}
