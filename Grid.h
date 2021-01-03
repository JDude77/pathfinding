#pragma once
#include <iostream>
#include <list>
#include <unordered_set>
#include <vector>
#include "AStar.h"
#include "Lee.h"
#include "Node.h"

using namespace std;
class Grid
{
public:

	enum PATHMODE
	{
		ASTAR,
		LEE
	};

	int pathMode = LEE;

	void displayGrid();
	void initialiseRandomGrid();
	list<Node *> getPath();
	void findPath();
	void printPath();
	void cleanUp();
	

	virtual ~Grid();
	
private:
	const static int width =			50;
	const static int height	=			25;
	int obstacleChance =				0;

	int grid[height][width];

	Node *start;
	Node *end;

	list<Node *> path;

	AStar aStar;
	Lee lee;

	void destroyPath();
};