#include "AStar.h"

void AStar::findPath(int grid[maxHeight][maxWidth], int height, int width, Node* start, Node* end)
{
	//Create the structures that hold the open and closed data sets
	vector<Node*> openSet;
	unordered_set<Node*> closedSet;

	bool found = false;

	//Create start node properly (the x and y are created in the initialisation of the grid)
	start->h = fabs(end->x - start->x) + fabs(end->y - start->y);
	start->g = 0;
	start->f = start->h + start->g;
	start->parentNode = nullptr;

	//Make the start node the first one in the open set
	openSet.push_back(start);

	//Create new node for holding all the info as we loop through stuff
	Node* currentNode = NULL;

	//While we haven't run out of potential paths
	while (!openSet.empty())
	{
		//Move the next current node from the open set to the closed set
		currentNode = openSet.back();

		//If the node is the destination
		if (*currentNode == *end)
		{
			//Set all of end's values manually
			//(Doing direct equals of nodes makes the end node get erased on the sets being cleared)
			end->h = currentNode->h;
			end->g = currentNode->g;
			end->f = currentNode->f;
			end->parentNode = currentNode->parentNode;
			found = true;
			break;
		}//End if

		openSet.pop_back();
		closedSet.insert(currentNode);

		//If not the destination, check all surrounding valid nodes
		checkNeigbouringNodes(currentNode, &openSet, &closedSet, grid, start, end, width, height);
	}//End while

	//Trace backwards through all of the parent nodes to gather the path values, if a path has been found
	if (found)
	{
		currentNode = end;
		while (currentNode->parentNode != nullptr)
		{
			path.push_front(currentNode);
			currentNode = currentNode->parentNode;
		}//End while

		//Add the start node to the path manually, since it has no parent but is still part of the path
		path.push_front(start);
	}//End if

	//If a path has not been found, throw an in-program error
	else
	{
		cout << "No Valid Path Found" << endl;
	}//End else

	//Loop through the closed set and delete every unused, non-path node
	for (Node* currentNode : openSet)
	{
		if (!nodeIsInPath(currentNode, start, end))
		{
			delete currentNode;
		}//End if
	}//End for

	//Clear the pointers in the open set
	openSet.clear();

	//Loop through the closed set and delete every unused, non-path node
	for (Node* currentNode : closedSet)
	{
		if (!nodeIsInPath(currentNode, start, end))
		{
			delete currentNode;
		}//End if
	}//End for

	//Clear the pointers in the closed set
	closedSet.clear();
}

void AStar::checkNeigbouringNodes(Node* currentNode, vector<Node*>* openSet, unordered_set<Node*>* closedSet, int grid[maxHeight][maxWidth], Node* start, Node* end, int width, int height)
{
	bool validAbove, validBelow, validLeft, validRight;
	//Check the nodes to the left and right of the current node if in range
	for (int check = -1; check <= 1; check += 2)
	{
		//Reset booleans to be false each iteration
		validAbove = false; validBelow = false; validLeft = false; validRight = false;

		//Checking above and left
		if (check == -1)
		{
			validAbove = currentNode->y + check != -1 && grid[currentNode->y + check][currentNode->x] != -2;
			validLeft = currentNode->x + check != -1 && grid[currentNode->y][currentNode->x + check] != -2;
		}//End if

		//Checking below and right
		else
		{
			validBelow = currentNode->y + check != height && grid[currentNode->y + check][currentNode->x] != -2;
			validRight = currentNode->x + check != width && grid[currentNode->y][currentNode->x + check] != -2;
		}//End else

		if (validAbove || validBelow)	 checkNeighbour(check, 'Y', currentNode, openSet, closedSet, grid, start, end);
		if (validLeft || validRight)	 checkNeighbour(check, 'X', currentNode, openSet, closedSet, grid, start, end);
	}//End for
}

void AStar::checkNeighbour(int checkSide, char axis, Node* currentNode, vector<Node*>* openSet, unordered_set<Node*>* closedSet, int grid[maxHeight][maxWidth], Node* start, Node* end)
{
	Node* newNode = new Node();

	if (axis == 'Y')
	{
		newNode->x = currentNode->x;
		newNode->y = currentNode->y + checkSide;
	}//End if

	else if (axis == 'X')
	{
		newNode->x = currentNode->x + checkSide;
		newNode->y = currentNode->y;
	}//End else if

	//If there isn't, it's worth considering
	if (!addNodeToOpenSet(*closedSet, newNode, *openSet, currentNode, grid, start, end))
	{
		delete newNode;
	}//End if
}

void AStar::insertNodeAStar(vector<Node*>& openSet, Node* nodeToAdd)
{
	//Loop through the open set and find the correct point to add the new node 
	vector<Node*>::iterator insertionPoint = std::lower_bound(openSet.begin(), openSet.end(), nodeToAdd,
		[](const Node* lhs, const Node* rhs) { return lhs->f > rhs->f;});

	openSet.insert(insertionPoint, nodeToAdd);
}

bool AStar::addNodeToOpenSet(std::unordered_set<Node*>& closedSet, Node*& newNode, std::vector<Node*>& openSet, Node* currentNode, int grid[maxHeight][maxWidth], Node* start, Node* end)
{
	//If the node is already in the closed set, don't bother checking the open set
	bool foundClosed = find_if(closedSet.begin(), closedSet.end(), [newNode](const Node* compareNode) { return compareNode->x == newNode->x && compareNode->y == newNode->y; }) != closedSet.end();

	if (!foundClosed)
	{
		//Check to see if the node is already in the open set
		bool foundOpen = find_if(openSet.begin(), openSet.end(), [newNode](const Node* compareNode) { return compareNode->x == newNode->x && compareNode->y == newNode->y; }) != openSet.end();

		//If it isn't
		if (!foundOpen)
		{
			//Calculate its values
			calculateNodeValuesAStar(currentNode, newNode, start, end);

			//Update the visual on the grid, using the distance from the start point as a visual to say it's been considered
			grid[newNode->y][newNode->x] = newNode->g;

			//Add the node to the set
			insertNodeAStar(openSet, newNode);

			return true;
		}//End if
	}//End if

	return false;
}

void AStar::calculateNodeValuesAStar(Node* current, Node* newNode, Node* start, Node* end)
{
	//Distance from start to here
	newNode->g = current->g + fabs(newNode->y - current->y) + fabs(newNode->x - current->x);
	//Distance from here to end
	newNode->h = fabs(end->x - newNode->x) + fabs(end->y - newNode->y);
	//Total f value
	newNode->f = newNode->g + newNode->h;

	//New node's parent is current node
	newNode->parentNode = current;
}

bool AStar::nodeIsInPath(Node *& currentNode, Node* start, Node* end)
{
	//Node is in path or node is the start or end for cases where there is no path
	return currentNode == start || currentNode == end || find(path.begin(), path.end(), currentNode) != path.end();
}

void AStar::destroyPath(Node* start, Node* end)
{
	//Clear the path
	for (Node* currentNode : path)
	{
		if (currentNode != start && currentNode != end)
		{
			delete currentNode;
		}
	}//End for
	path.clear();
}

void AStar::printPath()
{
	cout << "PATH: " << endl;
	if (!path.empty())
	{
		for (Node* c : path)
		{
			cout << "- " << c->x << ", " << c->y << endl;
		}
	}
	else cout << "PATH NOT FOUND" << endl;
}

list<Node*> AStar::getPath()
{
	return path;
}
