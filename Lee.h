#pragma once
#include <list>
#include <iostream>
#include "Node.h"

using namespace std;
class Lee
{
private:
	const static int maxWidth = 100;
	const static int maxHeight = 75;

	list<Node*> path;

	bool floodGridLee(int grid[maxHeight][maxWidth], Node* start, Node* end, int width, int height);

public:
	void findPath(int grid[maxHeight][maxWidth], Node* start, Node* end, int width, int height);
	void destroyPath(Node* start, Node* end);
	void printPath();
	list<Node*> getPath();
};