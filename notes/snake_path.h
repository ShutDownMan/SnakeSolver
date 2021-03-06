#ifndef SNAKE_PATH_H
#define SNAKE_PATH_H

#define WHITE 0
#define BLACK 1

typedef struct hamiltonianNode {
	unsigned char edges[4]; //< 0 left -> goes with rotation
} HamiltonianNode;

typedef struct gridGraph {
	// array of nodes
	HamiltonianNode ***hamNodes;
	unsigned int length, width;

	unsigned char rotation; //< 1 clockwise 0 anticlockwise
} GridGraph;

typedef struct position {
	unsigned int x, y;
	unsigned int qPosition;
} Position;

typedef struct pathInfo {
	unsigned char **tag;
	char **direction;
	unsigned int **cost;
	unsigned int length, width;
} PathInfo;

GridGraph *createGridGraph(unsigned int length, unsigned int width, unsigned int rotation);

void createEdge(GridGraph *gridGraph, Position nodeA, Position nodeB);

char getDirection(Position nodeA, Position nodeB); /// ABR

unsigned char isValidPosition(GridGraph *gridGraph, Position pos);

unsigned int getDirection(GridGraph *gridGraph, Position src, Position dst);

PathInfo *createPathInfo(unsigned int length, unsigned int width);

unsigned int calcPathRec(GridGraph *gridGraph, PathInfo *pathInfo, Position src, Position dst);

/// ABD
void getAdjacentNode(GridGraph *gridGraph, Position &adjNode, Position source, int direction);

int isFreeNode(GridGraph *gridGraph, Position node);

unsigned int getBranchLength(GridGraph *gridGraph, Position src, int direction);

unsigned int getBranchLengthRec(GridGraph *gridGraph, PathInfo *pathInfo, Position src);

void printPathInfo(PathInfo *pathInfo);

void deletePathInfo(PathInfo *pathInfo);

char getKeyFromDirection(char qPosition, char direction);

void showGraph(GridGraph *gridGraph);

#endif