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
		~Node();
		bool operator == (const Node& rhs) const;
	};

	void displayGrid();
	void initialiseRandomGrid();
	list<Node *> getPath();
	void printPath();
	void resetGrid();
	void findPathLee();
	void findPathAStar();
	
private:
	const static int width =			100;
	const static int height	=			100;
	const static int obstacleChance =	0;

	int grid[width][height];

	Node start;
	Node end;

	list<Node *> path;

	bool floodGridLee();

	void checkNeigbouringNodes(Node* currentNode, vector<Node*>* openSet, unordered_set<Node*>* closedSet);
	void checkNeighbour(int checkSide, char axis, Node* currentNode, vector<Node*>* openSet, unordered_set<Node*>* closedSet);
	void insertNodeAStar(vector<Node*>& openSet, Node* nodeToAdd);
	void addNodeToOpenSet(std::unordered_set<Grid::Node*>& closedSet, Grid::Node*& newNode, std::vector<Grid::Node*>& openSet, Grid::Node* currentNode);
	void calculateNodeValuesAStar(Node* current, Node* newNode);
};


