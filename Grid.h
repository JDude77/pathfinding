#pragma once
#include <iostream>
#include <list>
#include <unordered_set>
#include <vector>

using namespace std;
class Grid
{
public:
	struct Node
	{
		int x = -1;
		int y = -1;
		double f, g, h;
		Node* parentNode;
		bool operator == (const Node& rhs) const;
	};

	void displayGrid();
	void initialiseGrid();
	void printPath();

	void floodGridLee();
	list<Node*> createPathLee();

	void findPathAStar();
	void addNodeToOpenSet(std::unordered_set<Grid::Node*>& closedSet, Grid::Node*& newNode, std::vector<Grid::Node*>& openSet, Grid::Node* currentNode);
	void insertNodeAStar(vector<Node*>& openSet, Node* nodeToAdd);
	void calculateNodeValuesAStar(Node* current, Node* newNode);

private:
	const static int width = 10;
	const static int height = 10;

	int grid[width][height];

	Node start;
	Node end;

	list<Node *> path;
};


