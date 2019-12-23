#include "./input.h"

int kbhit(void)
{
	int ch = getch();
	if (ch != ERR) {
		// such a weird feature, go back char we dont want you here
		ungetch(ch);
		return 1;
	} else {
		return 0;
	}
}

directions eval(char c)
{
	// parses wasd as directional keys
	/// TODO parse arrow keys as directionals?
	switch (c) {
	case 'a':
		return W;
	case 's':
		return S;
	case 'd':
		return E;
	case 'w':
		return N;
	default:
		return INVALID;
	}
}

void directionalIncrease(int *x, int *y, directions dir)
{
	// adapt the value according to the direction provided
	// as to provide the value of a movement in the provided direction
	if (dir == N) {
		(*y)--;
	} else if (dir == E) {
		(*x)++;
	} else if (dir == S) {
		(*y)++;
	} else if (dir == W) {
		(*x)--;
	}
}

directions userInput(directions dir)
{
	cbreak();	       // disables line buffering
	nodelay(stdscr, TRUE); // disables getch waiting lock

	if (kbhit()) {
		char c = getch();

		// only gets the last char on the input buffer
		while (kbhit()) {
			c = getch();
		}

		directions result = eval(c);

		// verifies if direction parallel or transverse
		if (result == INVALID || result % 2 == dir % 2) {
			return dir;
		}

		return result;
	} else {
		return dir;
	}
}

directions snakeAIInput(char dir)
{
	switch (dir) {
	case 0:
		return W;
	case 3:
		return S;
	case 2:
		return E;
	case 1:
		return N;
	default:
		return INVALID;
	}
}
