#pragma once
#include <vector>
#include <unordered_set>
#include <list>
#include <iostream>
#include "Node.h"

using namespace std;

class AStar
{
	private:
		const static int maxWidth = 100;
		const static int maxHeight = 75;

		list<Node*> path;

		void checkNeigbouringNodes(Node* currentNode, vector<Node*>* openSet, unordered_set<Node*>* closedSet, int grid[maxHeight][maxWidth], Node* start, Node* end, int width, int height);
		void checkNeighbour(int checkSide, char axis, Node* currentNode, vector<Node*>* openSet, unordered_set<Node*>* closedSet, int grid[maxHeight][maxWidth], Node* start, Node* end);
		void insertNodeAStar(vector<Node*>& openSet, Node* nodeToAdd);
		bool addNodeToOpenSet(std::unordered_set<Node*>& closedSet, Node*& newNode, std::vector<Node*>& openSet, Node* currentNode, int grid[maxHeight][maxWidth], Node* start, Node* end);
		void calculateNodeValuesAStar(Node* current, Node* newNode, Node* start, Node* end);
		bool nodeIsInPath(Node *& currentNode, Node* start, Node* end);

	public:
		void findPath(int grid[maxHeight][maxWidth], int height, int width, Node* start, Node* end);
		void destroyPath(Node* start, Node* end);
		void printPath();
		list<Node*> getPath();
};