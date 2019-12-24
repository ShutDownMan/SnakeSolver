// #include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <math.h>

#include "snake_path.h"

PathInfo *branchLengthPathInfo;

PathInfo *directionPathInfo;

int not_main(int argc, char const *argv[]) {

	GridGraph *gridGraph = createGridGraph(7, 6, 0);

	generatePathInfo(gridGraph);

	// createEdge(gridGraph, (Position){.x = 0, .y = 1}, (Position){.x = 0, .y = 0});
	// createEdge(gridGraph, (Position){.x = 1, .y = 0}, (Position){.x = 0, .y = 0});
	createEdge(gridGraph, (Position){.x = 0, .y = 0}, (Position){.x = 0, .y = 1});
	createEdge(gridGraph, (Position){.x = 0, .y = 1}, (Position){.x = 1, .y = 1});
	createEdge(gridGraph, (Position){.x = 1, .y = 1}, (Position){.x = 1, .y = 2});
	createEdge(gridGraph, (Position){.x = 2, .y = 1}, (Position){.x = 2, .y = 0});
	createEdge(gridGraph, (Position){.x = 1, .y = 2}, (Position){.x = 0, .y = 2});
	createEdge(gridGraph, (Position){.x = 1, .y = 1}, (Position){.x = 2, .y = 1});
	createEdge(gridGraph, (Position){.x = 2, .y = 1}, (Position){.x = 2, .y = 2});
	createEdge(gridGraph, (Position){.x = 2, .y = 0}, (Position){.x = 3, .y = 0});
	createEdge(gridGraph, (Position){.x = 2, .y = 2}, (Position){.x = 3, .y = 2});
	createEdge(gridGraph, (Position){.x = 3, .y = 2}, (Position){.x = 3, .y = 1});

	// createEdge(gridGraph, (Position){.x = 1, .y = 1}, (Position){.x = 1, .y = 2});
	// createEdge(gridGraph, (Position){.x = 1, .y = 1}, (Position){.x = 2, .y = 1});
	// createEdge(gridGraph, (Position){.x = 2, .y = 1}, (Position){.x = 2, .y = 2});
	// printf("AStar = %d\n\r", calcPathAStar(gridGraph, directionPathInfo,
	// 	(Position){.x = 4, .y = 0, .qPosition = 2},
	// 	(Position){.x = 0, .y = 0, .qPosition = 0}));
	// printPathInfo(directionPathInfo);
	printf("Direction = %d\n\r", getNextDirection(gridGraph, (Position){.x = 0, .y = 0, .qPosition = 2},
		(Position){.x = 3, .y = 3, .qPosition = 0}));

	// printf("Key = %d\n\r", getKeyFromDirection(1, 3));

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
	char direction = getDirection(nodeA, nodeB);

	if(direction < 0) return;

	if(abs(nodeA.x - nodeB.x) + abs(nodeA.y - nodeB.y) > 1) return;

	// mvprintw(30, 0, "\r(%2d, %2d) => (%2d, %2d)\n", nodeA.x, nodeA.y, nodeB.x, nodeB.y);
	// mvprintw(31, 0, "\rf = %g\n", fabs(nodeA.x*1.0 - nodeB.x) + fabs(nodeA.y*1.0 - nodeB.y));

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

void generatePathInfo(GridGraph *gridGraph) {
	directionPathInfo = createPathInfo(gridGraph->length, gridGraph->width);
	branchLengthPathInfo = createPathInfo(gridGraph->length, gridGraph->width);
}

void cleanPathInfo(PathInfo *pathInfo) {
	int i, j, k;

	for(i = 0; i < pathInfo->length; ++i) {
		for(j = 0; j < pathInfo->width; ++j) {
			pathInfo->tag[i][j] = 0;
			pathInfo->direction[i][j] = -1;
			pathInfo->hCost[i][j] = 0xFFFF;
			pathInfo->gCost[i][j] = 0xFFFF;
			pathInfo->cost[i][j] = 0xFFFF;
		}
	}
}

unsigned int getNextDirection(GridGraph *gridGraph, Position src, Position dest) {
	int direction;

	directionPathInfo->tag[src.y][src.x] = BLACK;

	calcPathRec(gridGraph, directionPathInfo, src, dest);

	// printPathInfo(directionPathInfo);

	direction = directionPathInfo->direction[src.y][src.x];

	/// clean directionPathInfo
	cleanPathInfo(directionPathInfo);

	return direction;
}

PathInfo *createPathInfo(unsigned int length, unsigned int width) {
	PathInfo *newPathInfo = (PathInfo *)malloc(sizeof(PathInfo));

	newPathInfo->length = length;
	newPathInfo->width = width;

	newPathInfo->tag = (unsigned char **)malloc(sizeof(unsigned char *) * length);
	newPathInfo->direction = (char **)malloc(sizeof(char *) * length);
	newPathInfo->cost = (unsigned int **)malloc(sizeof(unsigned int *) * length);
	newPathInfo->hCost = (unsigned int **)malloc(sizeof(unsigned int *) * length);
	newPathInfo->fCost = (unsigned int **)malloc(sizeof(unsigned int *) * length);
	newPathInfo->gCost = (unsigned int **)malloc(sizeof(unsigned int *) * length);
	for (int i = 0; i < length; ++i) {
		newPathInfo->tag[i] = (unsigned char *)malloc(sizeof(unsigned char) * width);
		newPathInfo->direction[i] = (char *)malloc(sizeof(char) * width);
		newPathInfo->cost[i] = (unsigned int *)malloc(sizeof(unsigned int) * width);
		newPathInfo->gCost[i] = (unsigned int *)malloc(sizeof(unsigned int) * width);
		newPathInfo->fCost[i] = (unsigned int *)malloc(sizeof(unsigned int) * width);
		newPathInfo->hCost[i] = (unsigned int *)malloc(sizeof(unsigned int) * width);

		for (int j = 0; j < width; ++j) {
			newPathInfo->tag[i][j] = 0;
			newPathInfo->direction[i][j] = -1;
			newPathInfo->cost[i][j] = 0xFFFF;
			newPathInfo->fCost[i][j] = 0xFFFF;
			newPathInfo->gCost[i][j] = 0xFFFF;
			newPathInfo->hCost[i][j] = 0xFFFF;
		}
	}

	return newPathInfo;
}

unsigned int calcPathRec(GridGraph *gridGraph, PathInfo *pathInfo, Position src, Position dest) {
	int i, j, minDir, curDist, branchLen, branchesLen, pathCost;
	char isSrcFree, isNextFree, firstBranch;

	curDist = branchLen = branchesLen = 0;
	minDir = firstBranch = -1;

	isSrcFree = isFreeNode(gridGraph, src);

	/// found node
	if(src.x == dest.x && src.y == dest.y) {
		// printf("FOUND!\n\r");
		// printf("(%d, %d) = (%d)\n\r", src.x, src.y, pathInfo->parent[src.y][src.x]);
		/// Add remaining branches in the node
		for(j = src.qPosition; j != dest.qPosition % 4; j = (j + 1) % 4) {
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
		firstBranch = -1;
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
			// printf("isNextFree = %d\n\r", isNextFree);

			/// if current node has an edge to that direction OR next node is a free node
			if(gridGraph->hamNodes[src.y][src.x]->edges[i] || isNextFree) {

				/// if next node is not tag black
				if(pathInfo->tag[nextNode.y][nextNode.x] != BLACK) {
					branchesLen = 0;
					// minDir = (minDir == -1) ? i : -1;
					// printf("tag = %d\n\r", gridGraph->hamNodes[nextNode.y][nextNode.x]->tag);
					for(j = (src.qPosition + 1) % 4; j != getDirection(src, nextNode); j = (j + 1) % 4) {

						branchesLen += 2 * (branchLen = getBranchLength(gridGraph, src, j));

						if(branchLen && firstBranch == -1) {
							firstBranch = j;
							// printf("\rFIRST BRANCH %d\n\r", firstBranch);
						}
						// if(src.x == 1 && src.y == 1) {
						// 	printf("j = %2d\n", j);
						// }
					}


					if(isNextFree) {
						resetAStar(pathInfo);
						pathInfo->gCost[nextNode.y][nextNode.x] = 1 + branchesLen;
						pathCost = calcPathAStar(gridGraph, pathInfo, dest, nextNode);
						pathInfo->direction[nextNode.y][nextNode.x] = getDirection(src, nextNode);
						// printf("FCOST = %d\n", pathCost);
						// pathInfo->tag[nextNode.y][nextNode.x] = WHITE;
					} else {
						pathInfo->tag[nextNode.y][nextNode.x] = BLACK;
						pathCost = calcPathRec(gridGraph, pathInfo, nextNode, dest);
					}

					/// get the distance recursively depth first
					curDist = 1 + branchesLen + pathCost;

					// if(src.x == 1 && src.y == 1) {
					// 	printf("\rcN = (%d, %d, %d)\n\r", src.x, src.y, src.qPosition);
					// 	printf("\rnN = (%d, %d, %d)\n\r", nextNode.x, nextNode.y, nextNode.qPosition);
					// 	printf("\rFIRSTBRANCH %d\n\r", firstBranch);
					// 	printf("\rBRANCHESLEN %d\n\r", branchesLen);
					// 	printf("\rCURDIST %d\n\r", curDist);
					// 	printf("\r---------------------------\n\r");
					// }

					// printf("minDist = %d\n\r", minDist);

					// printf("[%d, %d]\n\r", curDist, minDist);
					/// if current distance is smaller
					/// checks if distance is equal and avoids free nodes's paths
					if(!(curDist == pathInfo->cost[src.y][src.x] && isSrcFree) && curDist <= pathInfo->cost[src.y][src.x]) {
						pathInfo->cost[src.y][src.x] = curDist;

						minDir = i;
						//printPathInfo(pathInfo);

						// if(!isSrcFree) {
						// 	printf("curDist = %d\n\r", curDist);
						// 	printf("minDir = %d\n\r", minDir);
						// }
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

	if(!gridGraph->hamNodes[src.y][src.x]->edges[direction]) return 0;

	getAdjacentNode(gridGraph, &nextNode, src, direction);

	if(!isValidPosition(gridGraph, nextNode)) return 0;

	branchLengthPathInfo->tag[src.y][src.x] = BLACK;

	total = 1 + getBranchLengthRec(gridGraph, branchLengthPathInfo, nextNode);

	// untagGraph(gridGraph);

	cleanPathInfo(branchLengthPathInfo);

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
	printf("FCOST: \n\r");
	for(i = 0; i < pathInfo->length; ++i) {
		for(j = 0; j < pathInfo->width; ++j) {
			if(pathInfo->fCost[i][j] != -1) {
				printf("%2d ", pathInfo->fCost[i][j]);
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
			printw("#");
			if(gridGraph->hamNodes[i][j]->edges[2]) { // ABR
				printw("---");
			} else {
				printw("   ");
			}
		}
		printw("\n\r");
		for (j = 0; j < gridGraph->width; ++j) {
			if(gridGraph->hamNodes[i][j]->edges[3]) { // ABR
				printw("|   ");
			} else {
				printw("    ");
			}
		}
		printw("\n\r");
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

void resetAStar(PathInfo *pathInfo) {
	int i, j, k;

	for(i = 0; i < pathInfo->length; ++i) {
		for(j = 0; j < pathInfo->width; ++j) {
			pathInfo->gCost[i][j] = 0xFFFF;
			pathInfo->hCost[i][j] = 0xFFFF;
			pathInfo->fCost[i][j] = 0xFFFF;
		}
	}
}

Position *createPosition(int x, int y) {
	Position *newPos = (Position *)malloc(sizeof(Position));

	newPos->x = x;
	newPos->y = y;

	return newPos;
}

unsigned int calcPathAStar(GridGraph *gridGraph, PathInfo *pathInfo, Position src, Position dest) {
/*
	OPEN
	CLOSED

	insert OPEN src
	loop
		current = lowest fCost node in open
		remove OPEN current
		insert CLOSED current

		if current == dest
			return

		foreach adge of current
			if not valid OR not free
				continue

			if new path to edge < current path OR not contain open edge
				set new path cost and direction
				insert OPEN edge
*/
	ArrayList *open = arraylist_new(0);
	ArrayList *closed = arraylist_new(0);

	Position *curNode, adjNode;
	int i, newPathGCost, newPathHCost, newPathFCost;

	pathInfo->gCost[src.y][src.x] = 0;
	pathInfo->hCost[src.y][src.x] = abs(src.x - dest.x) + abs(src.y - dest.y);
	pathInfo->fCost[src.y][src.x] = pathInfo->hCost[src.y][src.x];

	arraylist_append(open, createPosition(src.x, src.y));

	while(TRUE) {
		arraylist_sort(open, compareNode);

		// printList(open);
		// printf("------------------\n");

		if(!open->length) {
			pathInfo->fCost[src.y][src.x] = 0xFFFF;
			break;
		}

		curNode = open->data[0];
		arraylist_remove(open, 0);
		arraylist_append(closed, curNode);

		if(curNode->x == dest.x && curNode->y == dest.y)
			break;

		for(i = 0; i < 4; ++i) {
			getAdjacentNode(gridGraph, &adjNode, *curNode, i);

			if(!isValidPosition(gridGraph, adjNode) || !isFreeNode(gridGraph, adjNode) ||
				arraylist_index_of(closed, equalNode, (void *)(&adjNode)) != -1)
				continue;

			newPathGCost = pathInfo->gCost[curNode->y][curNode->x] + 1;
			newPathHCost = abs(curNode->x - dest.x) + abs(curNode->y - dest.y);
			newPathFCost = newPathGCost + newPathHCost;

			if(newPathFCost < pathInfo->fCost[adjNode.y][adjNode.x] ||
					arraylist_index_of(open, equalNode, (void *)(&adjNode)) == -1) {

				pathInfo->gCost[adjNode.y][adjNode.x] = newPathGCost;
				pathInfo->hCost[adjNode.y][adjNode.x] = newPathHCost;
				pathInfo->fCost[adjNode.y][adjNode.x] = newPathFCost;

				pathInfo->direction[adjNode.y][adjNode.x] = getDirection(adjNode, *curNode);

				arraylist_append(open, createPosition(adjNode.x, adjNode.y));
			}

		}

	}

	for(i = 0; i < open->length; i ++) {
		free(open->data[i]);
	}
	for(i = 0; i < closed->length; i ++) {
		free(closed->data[i]);
	}

	arraylist_free(open);
	arraylist_free(closed);

	return pathInfo->fCost[src.y][src.x];
}

int equalNode(void *nodeA, void *nodeB) {
	return ((Position *)nodeA)->x == ((Position *)nodeB)->x &&
			((Position *)nodeA)->y == ((Position *)nodeB)->y;
}

int compareNode(void *nodeA, void *nodeB) {
	if(!equalNode((Position *)nodeA, (Position *)nodeB)) {
		return (directionPathInfo->fCost[((Position *)nodeA)->y][((Position *)nodeA)->x]
			< directionPathInfo->fCost[((Position *)nodeB)->y][((Position *)nodeB)->x]) ? -1 : 1;
	} else {
		return 0;
	}
}

void printList(ArrayList *list) {
	int i;

	for(i = 0; i < list->length; i ++) {
		printf("(%2d, %2d)\n", ((Position *)list->data[i])->x, ((Position *)list->data[i])->y);
	}
}