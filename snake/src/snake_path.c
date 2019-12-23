#include <stdio.h>
#include <stdlib.h>

#include "snake_path.h"

int mainTeste(int argc, char const *argv[]) {

	GridGraph *gridGraph = createGridGraph(3, 4, 0);

	// createEdge(gridGraph, (Position){.x = 0, .y = 1}, (Position){.x = 0, .y = 0});
	// createEdge(gridGraph, (Position){.x = 0, .y = 0}, (Position){.x = 1, .y = 0});
	// createEdge(gridGraph, (Position){.x = 0, .y = 1}, (Position){.x = 1, .y = 1});
	// createEdge(gridGraph, (Position){.x = 1, .y = 1}, (Position){.x = 1, .y = 2});
	// createEdge(gridGraph, (Position){.x = 2, .y = 1}, (Position){.x = 2, .y = 0});
	// createEdge(gridGraph, (Position){.x = 1, .y = 2}, (Position){.x = 0, .y = 2});
	// createEdge(gridGraph, (Position){.x = 1, .y = 1}, (Position){.x = 2, .y = 1});
	// createEdge(gridGraph, (Position){.x = 2, .y = 1}, (Position){.x = 2, .y = 2});
	// createEdge(gridGraph, (Position){.x = 2, .y = 0}, (Position){.x = 3, .y = 0});
	// createEdge(gridGraph, (Position){.x = 2, .y = 2}, (Position){.x = 3, .y = 2});
	// createEdge(gridGraph, (Position){.x = 3, .y = 2}, (Position){.x = 3, .y = 1});

	printf("Direction = %d\n\r", getNextDirection(gridGraph, (Position){.x = 3, .y = 2, .qPosition = 0}, (Position){.x = 0, .y = 2, .qPosition = 2}));

	printf("Key = %d\n\r", getKeyFromDirection(1, 3));

	showGraph(gridGraph);

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
		}
	}

	return newGG;
}

void createEdge(GridGraph *gridGraph, Position nodeA, Position nodeB) {
	unsigned char direction = getDirection(nodeA, nodeB);

	if(direction < 0) return;

	if(fabs(nodeA.x - nodeB.x) + fabs(nodeA.y - nodeB.y) != 1) return;

	// printf("nA = (%d, %d)\n\r", nodeA.y, nodeA.x);
	// printf("nB = (%d, %d)\n\r", nodeB.y, nodeB.x);
	// printf("D = %d/%d\n\r", direction, (direction + 2) % 4);

	if(isValidPosition(gridGraph, nodeA) && isValidPosition(gridGraph, nodeB)) {
		gridGraph->hamNodes[nodeA.y][nodeA.x]->edges[direction] = 1;
		gridGraph->hamNodes[nodeB.y][nodeB.x]->edges[(direction + 2) % 4] = 1;
	}

	//printEdges(gridGraph->hamNodes[nodeA.y][nodeA.x]->edges);
	//printEdges(gridGraph->hamNodes[nodeB.y][nodeB.x]->edges);
}

/// ABR
char getDirection(Position nodeA, Position nodeB) {
	if(nodeA.x != nodeB.x) {
		return (nodeA.x > nodeB.x) ? 0 : 2;
	} else if(nodeA.y != nodeB.y){
		return (nodeA.y < nodeB.y) ? 3 : 1;
	} else {
		return -1;
	}
}

unsigned char isValidPosition(GridGraph *gridGraph, Position pos) {
	return (pos.x >= 0 && pos.y >= 0 && pos.x < gridGraph->width && pos.y < gridGraph->length);
}

unsigned int getNextDirection(GridGraph *gridGraph, Position src, Position dst) {
	int direction;
	PathInfo *pathInfo = createPathInfo(gridGraph->length, gridGraph->width);

	calcPathRec(gridGraph, pathInfo, src, dst);

	// printPathInfo(pathInfo);

	direction = pathInfo->direction[src.y][src.x];

	/// delete pathInfo
	deletePathInfo(pathInfo);

	return direction;
}

PathInfo *createPathInfo(unsigned int length, unsigned int width) {
	PathInfo *newPathInfo = (PathInfo *)malloc(sizeof(PathInfo));

	newPathInfo->length = length;
	newPathInfo->width = width;

	newPathInfo->tag = (unsigned char **)malloc(sizeof(unsigned char *) * length);
	newPathInfo->direction = (char **)malloc(sizeof(char *) * length);
	newPathInfo->cost = (unsigned int **)malloc(sizeof(unsigned int *) * length);
	for (int i = 0; i < length; ++i) {
		newPathInfo->tag[i] = (unsigned char *)malloc(sizeof(unsigned char) * width);
		newPathInfo->direction[i] = (char *)malloc(sizeof(char) * width);
		newPathInfo->cost[i] = (unsigned int *)malloc(sizeof(unsigned int) * width);

		for (int j = 0; j < width; ++j) {
			newPathInfo->tag[i][j] = 0;
			newPathInfo->direction[i][j] = -1;
			newPathInfo->cost[i][j] = 0xFFFF;
		}
	}

	return newPathInfo;
}

unsigned int calcPathRec(GridGraph *gridGraph, PathInfo *pathInfo, Position src, Position dst) {
	int i, j, minDir, curDist, branchLen;
	// int minDist;
	char isSrcFree, isNextFree, firstBranch;

	curDist = 0;
	minDir = firstBranch = -1;

	/// found node
	if(src.x == dst.x && src.y == dst.y) {
		// printf("FOUND!\n\r");
		// printf("(%d, %d) = (%d)\n\r", src.x, src.y, pathInfo->parent[src.y][src.x]);
		/// Add remaining branches in the node
		for(j = src.qPosition; j != dst.qPosition % 4; j = (j + 1) % 4) {
			curDist += 2 * getBranchLength(gridGraph, src, (j + 1) % 4);
			// printf("VAL[%d] = %d\n\r", j, getBranchLength(gridGraph, src, (j + 1) % 4));
			// printf("qDir = %d\n\r", (j + 1) % 4);
			// printf("len = %d\n\r", getBranchLength(gridGraph, src, (j + 1) % 4));
		}

		/// if current path cost less
		if(curDist < pathInfo->cost[src.y][src.x]) {
			/// save cost
			pathInfo->cost[src.y][src.x] = curDist;
		}
		pathInfo->direction[src.y][src.x] = -1;

		return curDist;
	}

	/// if current node is free = grey, otherwise it's black
	// pathInfo->tag[src.y][src.x] = (isSrcFree = isFreeNode(gridGraph, src)) ? GREY : BLACK;
	// pathInfo->tag[src.y][src.x] = (isSrcFree = isFreeNode(gridGraph, src)) ? WHITE : BLACK;
	// pathInfo->cost[src.y][src.x] = 0;
	/// for each direction
	for(i = 0; i < 4; ++i) {
		curDist = 0;
		// printf("edge[%d] = %d\n\r", i, gridGraph->hamNodes[src.y][src.x]->edges[i]);
		Position nextNode = {.x = 0, .y = 0, .qPosition = 0};

		/// get next node = adjacent edge given direction
		getAdjacentNode(gridGraph, &nextNode, src, i);
		/// if adjacent node is a valid position
		if(isValidPosition(gridGraph, nextNode)) {

			// if(pathInfo->parent[nextNode.y][nextNode.x] == -1)
			// 	pathInfo->parent[nextNode.y][nextNode.x] = src.y * gridGraph->width + src.x;

			/// get if next node is free
			isNextFree = isFreeNode(gridGraph, nextNode);
			// printf("nN = (%d, %d, %d)\n\r", nextNode.x, nextNode.y, nextNode.qPosition);
			// printf("isNextFree = %d\n\r", isNextFree);

			/// if current node has an edge to that direction OR next node is a free node
			if(gridGraph->hamNodes[src.y][src.x]->edges[i] || isNextFree) {

				/// if next node is not tag black
				if(pathInfo->tag[nextNode.y][nextNode.x] != BLACK) {
					// minDir = (minDir == -1) ? i : -1;
					// printf("tag = %d\n\r", gridGraph->hamNodes[nextNode.y][nextNode.x]->tag);
					for(j = src.qPosition; j != (nextNode.qPosition + 1) % 4; j = (j + 1) % 4) {

						curDist += 2 * (branchLen = getBranchLength(gridGraph, src, (j + 1) % 4));

						if(branchLen && firstBranch == -1 && gridGraph->hamNodes[src.y][src.x]->edges[i])
							firstBranch = (j + 1) % 4;
					}

					pathInfo->tag[nextNode.y][nextNode.x] = BLACK;

					/// get the distance recursively depth first
					curDist += 1 + calcPathRec(gridGraph, pathInfo, nextNode, dst);

					if(isNextFree)
						pathInfo->tag[nextNode.y][nextNode.x] = WHITE;

					// printf("minDist = %d\n\r", minDist);

					// printf("[%d, %d]\n\r", curDist, minDist);
					/// if current distance is smaller
					/// checks if distance is equal and avoids free nodes's paths
					if(!(curDist == pathInfo->cost[src.y][src.x] && isSrcFree) && curDist <= pathInfo->cost[src.y][src.x]) {
						pathInfo->cost[src.y][src.x] = curDist;

						minDir = i;
						//printPathInfo(pathInfo);

						//printf("curDist = %d\n\r", curDist);
						// printf("minDir = %d\n\r", minDir);
						// pathInfo->parent[nextNode.y][nextNode.x] = src.y * gridGraph->width + src.x;
						pathInfo->direction[src.y][src.x] = (firstBranch == -1) ? minDir : firstBranch;
					}
				}
			}
		}
	}

	pathInfo->tag[src.y][src.x] = BLACK;

	// pathInfo->cost[src.y][src.x] = minDist;

	return pathInfo->cost[src.y][src.x];
}

unsigned int getBranchLength(GridGraph *gridGraph, Position src, int direction) {
	unsigned int total = 0;
	Position nextNode = {.x = 0, .y = 0, .qPosition = 0};
	PathInfo *pathInfo = NULL;

	if(!gridGraph->hamNodes[src.y][src.x]->edges[direction]) return 0;

	getAdjacentNode(gridGraph, &nextNode, src, direction);

	if(!isValidPosition(gridGraph, nextNode)) return 0;

	pathInfo = createPathInfo(gridGraph->length, gridGraph->width);

	pathInfo->tag[src.y][src.x] = BLACK;

	total = 1 + getBranchLengthRec(gridGraph, pathInfo, nextNode);

	// untagGraph(gridGraph);

	deletePathInfo(pathInfo);

	return total;
}

unsigned int getBranchLengthRec(GridGraph *gridGraph, PathInfo *pathInfo, Position src) {
	int i, total;
	Position nextNode = {.x = 0, .y = 0, .qPosition = 0};

	pathInfo->tag[src.y][src.x] = BLACK;

	for (i = total = 0; i < 4; ++i) {
		// printf("cN = (%d, %d)\n\r", src.x, src.y);
		// printf("cP = %d -> (%d, %d)\n\r", gridGraph->hamNodes[src.y][src.x]->parent, src.x, src.y);
		// printf("edge[%d] = %d\n\r", i, gridGraph->hamNodes[src.y][src.x]->edges[i]);
		if(gridGraph->hamNodes[src.y][src.x]->edges[i]) {
			getAdjacentNode(gridGraph, &nextNode, src, i);
			// printf("nN = (%d, %d)\n\r", nextNode.x, nextNode.y);

			if(isValidPosition(gridGraph, nextNode) && !pathInfo->tag[nextNode.y][nextNode.x]) {
				// printf("nN = (%d, %d)\n\r", nextNode.x, nextNode.y);
				total += 1 + getBranchLengthRec(gridGraph, pathInfo, nextNode);
				// printf("A\n\r");
			}
		}
		// printf("total = %d\n\r", total);
	}

	// printf("cN = (%d, %d)\n\r", src.x, src.y);

	return total;
}

void getAdjacentNode(GridGraph *gridGraph, Position *adjNode, Position source, int direction) {
	adjNode->y = source.y;
	adjNode->x = source.x;

	adjNode->qPosition = (direction + 2) % 4;

	if(direction % 2) {
		adjNode->y += direction - 2;
	} else {
		adjNode->x += direction - 1;
	}
}

int isFreeNode(GridGraph *gridGraph, Position node) {
	int i, hasNeighbour;

	for(i = hasNeighbour = 0; !hasNeighbour && i < 4; ++i) {
		hasNeighbour = gridGraph->hamNodes[node.y][node.x]->edges[i];
	}

	return !hasNeighbour;
}

void printPathInfo(PathInfo *pathInfo) {
	int i, j;

	printf("COST: \n\r");
	for(i = 0; i < pathInfo->length; ++i) {
		for(j = 0; j < pathInfo->width; ++j) {
			printf("%5d ", pathInfo->cost[i][j]);
		}
		printf("\n\r");
	}
	printf("DIRECTION: \n\r");
	for(i = 0; i < pathInfo->length; ++i) {
		for(j = 0; j < pathInfo->width; ++j) {
			if(pathInfo->direction[i][j] != -1) {
				printf("%2d ", pathInfo->direction[i][j]);
			} else {
				printf("%2d ", -1);
			}
		}
		printf("\n\r");
	}
}

void deletePathInfo(PathInfo *pathInfo) {
	for (int i = 0; i < pathInfo->length; ++i) {
		free(pathInfo->tag[i]);
		free(pathInfo->direction[i]);
		free(pathInfo->cost[i]);
	}

	free(pathInfo->tag);
	free(pathInfo->direction);
	free(pathInfo->cost);

	free(pathInfo);
}

char getKeyFromDirection(char qPosition, char direction) {
	if((qPosition + 1) % 4 == direction)
		return direction;

	return (qPosition + 2) % 4;
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
		printf("\n\r");
		for (j = 0; j < gridGraph->width; ++j) {
			if(gridGraph->hamNodes[i][j]->edges[3]) { // ABR
				printf("|   ");
			} else {
				printf("    ");
			}
		}
		printf("\n\r");
	}
}

/// ABR
char getQPosition(int x, int y) {
	/// Transforms x, y coordinates into quadrant position
	if(y % 2) {
		return 3 - (x % 2);
	} else {
		return (x % 2);
	}
}

void cleanGraph(GridGraph *gridGraph) {
	int i, j, k;

	for(i = 0; i < gridGraph->length; ++i) {
		for(j = 0; j < gridGraph->width; ++j) {
			for(k = 0; k < 4; ++k) {
				gridGraph->hamNodes[i][j]->edges[k] = 0;
			}
		}
	}
}