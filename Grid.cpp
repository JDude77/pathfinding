#include "Grid.h"

#include <chrono>

using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::high_resolution_clock;

void Grid::displayGrid()
{
	//cout << "   0   | 1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9 " << endl;
	cout << "=============================================================================" << endl;
	for (int i = 0; i < width; i++)
	{
		//cout << i << " |";
		for (int j = 0; j < height; j++)
		{
			cout << grid[i][j] << "\t";
		}
		cout << "\n\n";
	}

	cout << "START: " << start.x << ", " << start.y << endl;
	cout << "END: " << end.x << ", " << end.y << endl;
}

void Grid::initialiseRandomGrid()
{
	auto time = std::chrono::high_resolution_clock::now();
	auto timeToNanoForSeed = duration_cast<nanoseconds>(time.time_since_epoch());

	srand((unsigned int)timeToNanoForSeed.count());

	//Random start and end place
	start.x = rand() % width;
	start.y = rand() % height;

	end.x = rand() % width;
	end.y = rand() % height;

	//To get more varied start and end points (and guarantee they're not occupying the same spot)
	if (start.x == end.x && start.y == end.y)
	{
		switch (rand() % 2)
		{
			case 1: while (start.x == end.x) end.x = rand() % width; break;
			default: while (start.y == end.y) end.y = rand() % height; break;
		}//End switch
	}//End if

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (rand() % 100 < obstacleChance)
			{
				grid[j][i] = -2;
			}
			else
			{
				grid[j][i] = -1;
			}
		}
	}

	//Set the display value for the start node to a distance of 0
	grid[start.y][start.x] = 0;

	//Make sure that the end value isn't an unreachable obstacle
	grid[end.y][end.x] = -1;
}

list<Grid::Node*> Grid::getPath()
{
	return path;
}

bool Grid::floodGridLee()
{
	int distance = 0;
	bool stuck = false, change = false;
	
	grid[start.y][start.x] = distance;

	while (grid[end.y][end.x] == -1 && !stuck)
	{
		change = false;
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				//Don't change the value of any obstacles
				if (grid[y][x] == -2) continue;

				//If the grid's value hasn't had an update yet
				if (grid[y][x] == -1)
				{
					//Check surrounding values, being sure to not go out of range
					//x + 1, y as is
					if (x < width-1)
					{
						if (grid[y][x + 1] == distance)
						{
							change = true;
							grid[y][x] = distance + 1;
							continue;
						}
					}
					
					//x - 1, y as is
					if (x != 0)
					{
						if (grid[y][x - 1] == distance)
						{
							change = true;
							grid[y][x] = distance + 1;
							continue;
						}
					}

					//x as is, y + 1
					if (y < height-1)
					{
						if (grid[y + 1][x] == distance)
						{
							change = true;
							grid[y][x] = distance + 1;
							continue;
						}
					}

					//x as is, y - 1
					if (y != 0)
					{
						if (grid[y - 1][x] == distance)
						{
							change = true;
							grid[y][x] = distance + 1;
							continue;
						}
					}
				}//End == -1 if
			}//End y for
		}//End x for

		if (!change) stuck = true;

		distance++;
	}//End while
	if (stuck) return false;
	else return true;
}

void Grid::findPathLee()
{
	path.clear();
	bool pathFound = floodGridLee();

	if (pathFound)
	{
		Node* currentPos = &end;
		int distance = grid[end.y][end.x];
		path.push_back(currentPos);

		while (distance != 0 && distance != -1 && distance != -2)
		{
			if (currentPos->x != 0)
			{
				if (grid[currentPos->y][currentPos->x - 1] == distance - 1)
				{
					Node* pos = new Node(*currentPos);
					pos->x = currentPos->x - 1;
					path.push_back(pos);
					currentPos = pos;
					distance--;
					continue;
				}
			}

			if (currentPos->y != 0)
			{
				if (grid[currentPos->y - 1][currentPos->x] == distance - 1)
				{
					Node* pos = new Node(*currentPos);
					pos->y = currentPos->y - 1;
					path.push_back(pos);
					currentPos = pos;
					distance--;
					continue;
				}
			}

			if (currentPos->x < width - 1)
			{
				if (grid[currentPos->y][currentPos->x + 1] == distance - 1)
				{
					Node* pos = new Node(*currentPos);
					pos->x = currentPos->x + 1;
					path.push_back(pos);
					currentPos = pos;
					distance--;
					continue;
				}
			}

			if (currentPos->y < height - 1)
			{
				if (grid[currentPos->y + 1][currentPos->x] == distance - 1)
				{
					Node* pos = new Node(*currentPos);
					pos->y = currentPos->y + 1;
					path.push_back(pos);
					currentPos = pos;
					distance--;
					continue;
				}
			}
		}//End while

		path.reverse();
	}//End if

	//If a path has not been found, throw an in-program error
	else
	{
		cout << "No Valid Path Found" << endl;
	}//End else
}

void Grid::findPathAStar()
{
	//Create the structures that hold the open and closed data sets
	vector<Node *> openSet;
	unordered_set<Node*> closedSet;

	bool found = false;

	//Create start node properly (the x and y are created in the initialisation of the grid)
	start.h = fabs(end.x - start.x) + fabs(end.y - start.y);
	start.g = 0;
	start.f = start.h + start.g;
	start.parentNode = nullptr;

	openSet.push_back(&start);

	//Create new node for holding all the info as we loop through stuff
	Node* currentNode = NULL;

	//While we haven't run out of potential paths
	while (!openSet.empty())
	{
		//Move the next current node from the open set to the closed set
		currentNode = openSet.back();
		openSet.pop_back();
		closedSet.insert(currentNode);

		//If the node is the destination
		if (*currentNode == end)
		{
			//Set all of end's values manually
			//(Doing direct equals of nodes makes the end node get erased on the sets being cleared)
			end.h = currentNode->h;
			end.g = currentNode->g;
			end.f = currentNode->f;
			end.parentNode = currentNode->parentNode;
			path.push_front(&end);
			found = true;
			break;
		}//End if

		//If not the destination, check all surrounding valid nodes
		checkNeigbouringNodes(currentNode, &openSet, &closedSet);
	}//End while

	//cout << "Open Set Size: " << openSet.size() << endl;
	//cout << "Closed Set Size: " << closedSet.size() << endl;

	//Clear the open and closed sets, since they are no longer needed

	//This works perfectly fine and deletes them all
	//data structure: vector
	for (Node* node : openSet)
	{
		delete(node);
		node = NULL;
	}

	//This causes the program to crash 
	//after a small number of iterations
	////data structure: unordered_set
	//for (Node* node : closedSet)
	//{
	//	delete(node);
	//	node = NULL;
	//}

	//Only calling these gets rid of the pointers,
	//but leaves the objects they pointed to intact
	openSet.clear();
	closedSet.clear();

	//Ensure the current path is empty (in case of algorithm being run multiple times)
	path.clear();

	//Trace backwards through all of the parent nodes to gather the path values, if a path has been found
	if (found)
	{
		currentNode = &end;
		while (currentNode->parentNode != nullptr)
		{
			path.push_front(currentNode);
			currentNode = currentNode->parentNode;
		}//End while
		path.push_front(&start);
	}//End if

	//If a path has not been found, throw an in-program error
	else
	{
		cout << "No Valid Path Found" << endl;
	}//End else
}

void Grid::checkNeigbouringNodes(Node* currentNode, vector<Node*> *openSet, unordered_set<Node*> *closedSet)
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
			validLeft = currentNode->x + check != -1 &&  grid[currentNode->y][currentNode->x + check] != -2;
		}//End if

		//Checking below and right
		else
		{
			validBelow = currentNode->y + check != height && grid[currentNode->y + check][currentNode->x] != -2;
			validRight = currentNode->x + check != width &&  grid[currentNode->y][currentNode->x + check] != -2;
		}//End else

		if (validAbove || validBelow)	 checkNeighbour(check, 'Y', currentNode, openSet, closedSet);
		if (validLeft  || validRight)	 checkNeighbour(check, 'X', currentNode, openSet, closedSet);
	}
}

void Grid::checkNeighbour(int checkSide, char axis, Node* currentNode, vector<Node*> *openSet, unordered_set<Node*> *closedSet)
{
	Node* newNode = new Node();
	if (axis == 'Y')
	{
		newNode->x = currentNode->x;
		newNode->y = currentNode->y + checkSide;
	}
	else if (axis == 'X')
	{
		newNode->x = currentNode->x + checkSide;
		newNode->y = currentNode->y;
	}
	
	addNodeToOpenSet(*closedSet, newNode, *openSet, currentNode);
}

void Grid::addNodeToOpenSet(std::unordered_set<Grid::Node*>& closedSet, Grid::Node*& newNode, std::vector<Grid::Node*>& openSet, Grid::Node* currentNode)
{
	//If the node is already in the closed set, don't bother checking the open set
	bool foundClosed = find_if(closedSet.begin(), closedSet.end(), [newNode](const Node* compareNode) { return compareNode->x == newNode->x && compareNode->y == newNode->y; }) != closedSet.end();
	if (foundClosed) return;

	//Check to see if the node is already in the open set
	bool notFoundOpen = find_if(openSet.begin(), openSet.end(), [newNode](const Node* compareNode) { return compareNode->x == newNode->x && compareNode->y == newNode->y; }) == openSet.end();

	//If it isn't, add it
	if (notFoundOpen)
	{
		//Calculate its values
		calculateNodeValuesAStar(currentNode, newNode);

		//Update the visual on the grid, using the distance from the start point as a visual to say it's been considered
		grid[newNode->y][newNode->x] = newNode->g;

		//Add the node to the set
		insertNodeAStar(openSet, newNode);
	}//End if
}

void Grid::insertNodeAStar(vector<Node*>& openSet, Node* nodeToAdd)
{
	//Loop through the open set and find the correct point to add the new node 
	vector<Node *>::iterator insertionPoint = std::lower_bound(openSet.begin(), openSet.end(), nodeToAdd,
		[](const Node* lhs, const Node *rhs) { return lhs->f > rhs->f; });

	openSet.insert(insertionPoint, nodeToAdd);
}

void Grid::calculateNodeValuesAStar(Node* current, Node* newNode)
{
	newNode->g = fabs(newNode->x - start.x) + fabs(newNode->y - start.y);
	newNode->h = fabs(end.x - newNode->x) + fabs(end.y - newNode->y);
	newNode->f = newNode->g + newNode->h;

	newNode->parentNode = current;
}

Grid::Node::~Node()
{
	parentNode = nullptr;
}

bool Grid::Node::operator == (const Grid::Node& rhs) const
{
	//Two nodes are considered the same if they have X and Y values that match each other
	return this->x == rhs.x && this->y == rhs.y;
}

void Grid::printPath()
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

void Grid::resetGrid()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (grid[j][i] == -2) continue;
			else
			{
				grid[j][i] = -1;
			}
		}
	}

	//Set the display value for the start node to a distance of 0
	grid[start.y][start.x] = 0;

	//Make sure that the end value isn't an unreachable obstacle
	grid[end.y][end.x] = -1;
}
