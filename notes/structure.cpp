#include <stdio.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

#define WHITE 0
#define BLACK 1
#define GREY  2

/*
	ABR = AFFECTED BY ROTATION

*/

typedef struct hamiltonianNode {
	unsigned char edges[4]; //< 0 left -> goes with rotation
	int parent;

	// unsigned char tagged;

	unsigned int height;
} HamiltonianNode;

typedef struct hamiltonianTree {
	HamiltonianNode *root;
} HamiltonianTree;

typedef struct gridGraph {
	// array of nodes
	HamiltonianNode ***hamNodes;
	unsigned int length, width;

	unsigned char rotation; //< 0 clockwise 1 anticlockwise
} GridGraph;

typedef struct position {
	unsigned int x, y;
	unsigned int qPosition;
} Position;

typedef struct snake {
	HamiltonianTree *snakeTree;

	HamiltonianTree *snakePath;

	Position snakeHead;
	unsigned char snakeHeadQPos; //< Quarter position within node, 0 top-left -> goes with rotation
} Snake;

typedef struct food {
	Position foodPosition;
	unsigned char foodQPos; //< Quarter position within node, 0 top-left -> goes with rotation
} Food;

typedef struct game {
	GridGraph *gridGraph;

	Snake *snake;

	Food *food;
} Game;

typedef struct tags {
	unsigned char **tagged;
	unsigned int **cost;
	int **parent;
	unsigned int length, width;
} Tags;

GridGraph *createGridGraph(unsigned int length, unsigned int width, unsigned int rotation);

Snake *createSnake(GridGraph *gridGraph);

Food *createFood(GridGraph *gridGraph);

void createEdge(GridGraph *gridGraph, Position nodeA, Position nodeB);

char getDirection(Position nodeA, Position nodeB); /// ABR

unsigned char isValidPosition(GridGraph *gridGraph, Position pos);

void showGraph(GridGraph *gridGraph);

void printEdges(unsigned char edges[4]);

unsigned int getDist(GridGraph *gridGraph, Position src, Position dst);

unsigned int getDistRec(GridGraph *gridGraph, Tags *tags, Position src, Position dst);

void getAdjacentNode(GridGraph *gridGraph, Position &adjNode, Position source, int direction); // ABD

int isFreeNode(GridGraph *gridGraph, Position node);

unsigned int getLength(GridGraph *gridGraph, Position src, int direction);

unsigned int getLengthRec(GridGraph *gridGraph, Tags *tags, Position src);

void untagGraph(GridGraph *gridGraph);


int main(int argc, char const *argv[]) {
	Game *game = (Game *)malloc(sizeof(Game));

	game->gridGraph = createGridGraph(3, 4, 1);
	game->snake = createSnake(game->gridGraph);
	game->food = createFood(game->gridGraph);

	untagGraph(game->gridGraph);

	createEdge(game->gridGraph, (Position){.x = 0, .y = 1}, (Position){.x = 0, .y = 0});
	createEdge(game->gridGraph, (Position){.x = 0, .y = 0}, (Position){.x = 1, .y = 0});
	createEdge(game->gridGraph, (Position){.x = 0, .y = 1}, (Position){.x = 1, .y = 1});
	createEdge(game->gridGraph, (Position){.x = 1, .y = 1}, (Position){.x = 1, .y = 2});
	createEdge(game->gridGraph, (Position){.x = 2, .y = 1}, (Position){.x = 2, .y = 0});
	createEdge(game->gridGraph, (Position){.x = 1, .y = 2}, (Position){.x = 0, .y = 2});
	createEdge(game->gridGraph, (Position){.x = 1, .y = 1}, (Position){.x = 2, .y = 1});
	// createEdge(game->gridGraph, (Position){.x = 2, .y = 1}, (Position){.x = 2, .y = 2});
	// createEdge(game->gridGraph, (Position){.x = 2, .y = 0}, (Position){.x = 3, .y = 0});
	// createEdge(game->gridGraph, (Position){.x = 2, .y = 2}, (Position){.x = 3, .y = 2});
	// createEdge(game->gridGraph, (Position){.x = 3, .y = 2}, (Position){.x = 3, .y = 1});

	printf("Distance = %d\n", getDist(game->gridGraph, (Position){.x = 1, .y = 1, .qPosition = 2}, (Position){.x = 3, .y = 1, .qPosition = 2}));

	printf("Length = %d\n", getLength(game->gridGraph, (Position){.x = 1, .y = 0}, 0));

	showGraph(game->gridGraph);

	return 0;
}

GridGraph *createGridGraph(unsigned int length, unsigned int width, unsigned int rotation) {
	GridGraph *newGG = (GridGraph *)malloc(sizeof(GridGraph));

	newGG->hamNodes = (HamiltonianNode ***)malloc(sizeof(HamiltonianNode **) * length);
	newGG->length = length;
	newGG->width = width;
	newGG->rotation = rotation;

	for (int i = 0; i < length; ++i) {
		newGG->hamNodes[i] = (HamiltonianNode **)malloc(sizeof(HamiltonianNode *) * width);
		for (int j = 0; j < width; ++j) {
			newGG->hamNodes[i][j] = (HamiltonianNode *)malloc(sizeof(HamiltonianNode));
			// newGG->hamNodes[i][j]->tagged = 0;
			newGG->hamNodes[i][j]->parent = -1;
		}
	}

	return newGG;
}

Snake *createSnake(GridGraph *gridGraph) {
	Snake *newSnake = (Snake *)malloc(sizeof(Snake));

	newSnake->snakeTree = NULL;
	newSnake->snakePath = NULL;
	newSnake->snakeHead = {.x = 0, .y = 0};
	newSnake->snakeHeadQPos = 0;

	return newSnake;
}

Food *createFood(GridGraph *gridGraph) {
	Food *newFood = (Food *)malloc(sizeof(Food));

	newFood->foodPosition = {.x = 0, .y = 0};
	newFood->foodQPos = 0;

	return newFood;
}

Tags *createTags(unsigned int length, unsigned int width) {
	Tags *newTags = (Tags *)malloc(sizeof(Tags));

	newTags->length = length;
	newTags->width = width;

	newTags->tagged = (unsigned char **)malloc(sizeof(unsigned char *) * length);
	newTags->cost = (unsigned int **)malloc(sizeof(unsigned int *) * length);
	newTags->parent = (int **)malloc(sizeof(int *) * length);
	for (int i = 0; i < length; ++i) {
		newTags->tagged[i] = (unsigned char *)malloc(sizeof(unsigned char) * width);
		newTags->cost[i] = (unsigned int *)malloc(sizeof(unsigned int) * width);
		newTags->parent[i] = (int *)malloc(sizeof(int) * width);

		for (int j = 0; j < width; ++j) {
			newTags->tagged[i][j] = 0;
			newTags->cost[i][j] = 0xFFFF;
			newTags->parent[i][j] = -1;
		}
	}

	return newTags;
}

void printTags(Tags *tags) {
	int i, j;

	printf("COST: \n");
	for(i = 0; i < tags->length; ++i) {
		for(j = 0; j < tags->width; ++j) {
			printf("%5d ", tags->cost[i][j]);
		}
		printf("\n");
	}
	printf("PARENT: \n");
	for(i = 0; i < tags->length; ++i) {
		for(j = 0; j < tags->width; ++j) {
			if(tags->parent[i][j] != -1) {
				printf("(%2d, %2d) ", tags->parent[i][j]%tags->width, (int)(tags->parent[i][j]/tags->width));
			} else {
				printf("(%2d, %2d) ", -1, -1);
			}
		}
		printf("\n");
	}
}

void deleteTags(Tags *tags) {
	for (int i = 0; i < tags->length; ++i) {
		free(tags->tagged[i]);
		free(tags->cost[i]);
		free(tags->parent[i]);
	}

	free(tags->tagged);
	free(tags->cost);
	free(tags->parent);

	free(tags);
}

void createEdge(GridGraph *gridGraph, Position nodeA, Position nodeB) {
	unsigned char direction = getDirection(nodeA, nodeB);

	if(direction < 0) return;

	// printf("nA = (%d, %d)\n", nodeA.y, nodeA.x);
	// printf("nB = (%d, %d)\n", nodeB.y, nodeB.x);
	// printf("D = %d/%d\n", direction, (direction + 2) % 4);

	if(isValidPosition(gridGraph, nodeA) && isValidPosition(gridGraph, nodeB)) {
		gridGraph->hamNodes[nodeA.y][nodeA.x]->edges[direction] = 1;
		gridGraph->hamNodes[nodeB.y][nodeB.x]->edges[(direction + 2) % 4] = 1;
		gridGraph->hamNodes[nodeB.y][nodeB.x]->parent = nodeA.y * gridGraph->width + nodeA.x;
	}

	//printEdges(gridGraph->hamNodes[nodeA.y][nodeA.x]->edges);
	//printEdges(gridGraph->hamNodes[nodeB.y][nodeB.x]->edges);
}

void printEdges(unsigned char edges[4]) {
	printf("edge = ");
	for (int i = 0; i < 4; ++i) {
		printf("%d ", edges[i]);
	}
	printf("\n");
}

char getDirection(Position nodeA, Position nodeB) { /// ABR
	if(nodeA.x != nodeB.x) {
		if(nodeA.x > nodeB.x) {
			return 0;
		} else {
			return 2;
		}
	} else if(nodeA.y != nodeB.y){
		if(nodeA.y < nodeB.y) {
			return 3;
		} else {
			return 1;
		}
	} else {
		return -1;
	}

}

unsigned char isValidPosition(GridGraph *gridGraph, Position pos) {
	return (pos.x >= 0 && pos.y >= 0 && pos.x < gridGraph->width && pos.y < gridGraph->length);
}

unsigned int getDist(GridGraph *gridGraph, Position src, Position dst) {
	int dist;
	Tags *tags = createTags(gridGraph->length, gridGraph->width);

	dist = getDistRec(gridGraph, tags, src, dst);
	//untagGraph(gridGraph);

	printTags(tags);

	// delete tags
	deleteTags(tags);

	return dist;
}

unsigned int getDistRec(GridGraph *gridGraph, Tags *tags, Position src, Position dst) {
	int i, j, minDir, curDist;
	int minDist;
	char isSrcFree, isNextFree;

	minDir = 0;
	minDist = 0xFFFF;

	/// found node OR node already visited
	if(src.x == dst.x && src.y == dst.y || tags->tagged[src.y][src.x] == GREY) {
		curDist = 0;
		// printf("FOUND!\n");
		// printf("(%d, %d) = (%d)\n", src.x, src.y, tags->parent[src.y][src.x]);
		/// Add remaining branches in the node
		for(j = src.qPosition; j != dst.qPosition % 4; ++j %= 4) {
			curDist += 2 * getLength(gridGraph, src, (j + 1) % 4);
			// printf("VAL[%d] = %d\n", j, getLength(gridGraph, src, (j + 1) % 4));
			// printf("qDir = %d\n", (j + 1) % 4);
			// printf("len = %d\n", getLength(gridGraph, src, (j + 1) % 4));
		}

		/// if current path cost less
		if(curDist < tags->cost[src.y][src.x]) {
			/// save cost
			tags->cost[src.y][src.x] = curDist;
		}

		return curDist;
	}

	/// if current node is free = grey, otherwise it's black
	tags->tagged[src.y][src.x] = (isSrcFree = isFreeNode(gridGraph, src)) ? GREY : BLACK;
	tags->cost[src.y][src.x] = 0;

	/// for each direction
	for(i = 0; i < 4; ++i) {
		curDist = 0;
		// printf("edge[%d] = %d\n", i, gridGraph->hamNodes[src.y][src.x]->edges[i]);
		Position nextNode = {.x = 0, .y = 0, .qPosition = 0};

		/// get next node = adjacent edge given direction
		getAdjacentNode(gridGraph, nextNode, src, i);
		/// if adjacent node is a valid position
		if(isValidPosition(gridGraph, nextNode)) {

			/// get if next node is free
			isNextFree = isFreeNode(gridGraph, nextNode);
			// printf("nN = (%d, %d, %d)\n", nextNode.x, nextNode.y, nextNode.qPosition);
			// printf("isNextFree = %d\n", isNextFree);

			/// if current node has an edge to that direction OR next node is a free node
			if(gridGraph->hamNodes[src.y][src.x]->edges[i] || isNextFree) {

				/// if next node is not tagged black
				if(tags->tagged[nextNode.y][nextNode.x] != BLACK) {
					// printf("tagged = %d\n", gridGraph->hamNodes[nextNode.y][nextNode.x]->tagged);
					for(j = src.qPosition; j != (nextNode.qPosition + 1) % 4; ++j %= 4) {
						curDist += 2 * getLength(gridGraph, src, (j + 1) % 4);
						//printf("qDir = %d\n", (j + 1) % 4);
						//printf("len = %d\n", getLength(gridGraph, src, (j + 1) % 4));
					}
					/// get the distance recursively depth first
					curDist += 1 + getDistRec(gridGraph, tags, nextNode, dst);

					// printf("minDist = %d\n", minDist);

					// printf("[%d, %d]\n", curDist, minDist);
					/// if current distance is smaller
					/// checks if distance is equal and avoids free nodes
					if(!(curDist == minDist && isSrcFree) && curDist <= minDist) {
						minDist = curDist;
						minDir = i;
						// printf("cN = (%d, %d)\n", src.x, src.y);
						printTags(tags);

						//printf("curDist = %d\n", curDist);
						// printf("minDir = %d\n", minDir);
						tags->parent[nextNode.y][nextNode.x] = src.y * gridGraph->width + src.x;
					}
				}
			}
		}
	}

	tags->tagged[src.y][src.x] = BLACK;
	tags->cost[src.y][src.x] = minDist;


	return minDist;
}

void getAdjacentNode(GridGraph *gridGraph, Position &adjNode, Position source, int direction) { // ABD
	adjNode.y = source.y;
	adjNode.x = source.x;

	adjNode.qPosition = (direction + 2) % 4;

	if(direction % 2) {
		adjNode.y += direction - 2;
	} else {
		adjNode.x += direction - 1;
	}
}

int isFreeNode(GridGraph *gridGraph, Position node) {
	int i, hasNeighbour;

	for(i = hasNeighbour = 0; !hasNeighbour && i < 4; ++i) {
		hasNeighbour = gridGraph->hamNodes[node.y][node.x]->edges[i];
	}

	return !hasNeighbour;
}

void sumPositions(Position &dest, Position source) {
	dest.x += source.x;
	dest.y += source.y;
}

unsigned int getLength(GridGraph *gridGraph, Position src, int direction) {
	unsigned int total = 0;
	Position nextNode = {.x = 0, .y = 0, .qPosition = 0};
	Tags *tags = NULL;

	if(!gridGraph->hamNodes[src.y][src.x]->edges[direction]) return 0;

	getAdjacentNode(gridGraph, nextNode, src, direction);

	if(!isValidPosition(gridGraph, nextNode)) return 0;

	tags = createTags(gridGraph->length, gridGraph->width);

	tags->tagged[src.y][src.x] = 1;

	total = 1 + getLengthRec(gridGraph, tags, nextNode);

	// untagGraph(gridGraph);

	deleteTags(tags);

	return total;
}

unsigned int getLengthRec(GridGraph *gridGraph, Tags *tags, Position src) {
	int i, total;
	Position nextNode = {.x = 0, .y = 0, .qPosition = 0};

	tags->tagged[src.y][src.x] = 1;

	for (i = total = 0; i < 4; ++i) {
		// printf("cN = (%d, %d)\n", src.x, src.y);
		// printf("cP = %d -> (%d, %d)\n", gridGraph->hamNodes[src.y][src.x]->parent, src.x, src.y);
		// printf("edge[%d] = %d\n", i, gridGraph->hamNodes[src.y][src.x]->edges[i]);
		if(gridGraph->hamNodes[src.y][src.x]->edges[i]) {
			getAdjacentNode(gridGraph, nextNode, src, i);
			// printf("nN = (%d, %d)\n", nextNode.x, nextNode.y);

			if(isValidPosition(gridGraph, nextNode) && !tags->tagged[nextNode.y][nextNode.x]) {
				// printf("nN = (%d, %d)\n", nextNode.x, nextNode.y);
				total += 1 + getLengthRec(gridGraph, tags, nextNode);
				// printf("A\n");
			}
		}
	}

	return total;
}

void untagGraph(GridGraph *gridGraph) {
	int i, j;

	for (i = 0; i < gridGraph->length; ++i) {
		for (j = 0; j < gridGraph->width; ++j) {
			// gridGraph->hamNodes[i][j]->tagged = 0;
		}
	}
}

void showGraph(GridGraph *gridGraph) {
	int i, j;

	for (i = 0; i < gridGraph->length; ++i) {
		for (j = 0; j < gridGraph->width; ++j) {
			printf("#");
			if(gridGraph->hamNodes[i][j]->edges[2]) { // ABR
				printf("---");
			} else {
				printf("   ");
			}
		}
		printf("\n");
		for (j = 0; j < gridGraph->width; ++j) {
			if(gridGraph->hamNodes[i][j]->edges[3]) { // ABR
				printf("|   ");
			} else {
				printf("    ");
			}
		}
		printf("\n");
	}

}