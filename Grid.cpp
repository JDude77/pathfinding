#include "Grid.h"

#include <stdlib.h>
#include <time.h>
#include <queue>
#include <unordered_set>

void Grid::displayGrid()
{
	cout << "   0   | 1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9 " << endl;
	cout << "=============================================================================" << endl;
	for (int i = 0; i < width; i++)
	{
		cout << i << " |";
		for (int j = 0; j < height; j++)
		{
			cout << grid[i][j] << "\t";
		}
		cout << "\n==|\n";
	}

	cout << "START: " << start.x << ", " << start.y << endl;
	cout << "END: " << end.x << ", " << end.y << endl;
}

void Grid::initialiseGrid()
{
	srand((unsigned int)time(NULL));

	//Random start and end place
	start.x = rand() % width;
	start.y = rand() % height;

	end.x = rand() % width;
	end.y = rand() % height;

	//To get more varied start and end points (and guarantee they're not occupying the same spot)
	while (start.x == end.x) end.x = rand() % width;
	while (start.y == end.y) end.y = rand() % height;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (rand() % 10 < 2)
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

void Grid::floodGridLee()
{
	int distance = 0;
	
	grid[start.y][start.x] = distance;

	while (grid[end.y][end.x] == -1)
	{
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
							grid[y][x] = distance + 1;
							continue;
						}
					}
					
					//x - 1, y as is
					if (x != 0)
					{
						if (grid[y][x - 1] == distance)
						{
							grid[y][x] = distance + 1;
							continue;
						}
					}

					//x as is, y + 1
					if (y < height-1)
					{
						if (grid[y + 1][x] == distance)
						{
							grid[y][x] = distance + 1;
							continue;
						}
					}

					//x as is, y - 1
					if (y != 0)
					{
						if (grid[y - 1][x] == distance)
						{
							grid[y][x] = distance + 1;
							continue;
						}
					}
				}//End == -1 if
			}//End y for
		}//End x for

		distance++;
	}
}

list<Grid::Node*> Grid::createPathLee()
{
	path.clear();
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

		if (currentPos->x < width-1)
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

		if (currentPos->y < height-1)
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

	return path;
}

void Grid::findPathAStar()
{
	//Create the structures that hold the open and closed data sets
	vector<Node*> openSet;
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
			found = true;
			break;
		}//End if

		//Check the nodes to the left and right of the current node if in range
		for (int cX = -1; cX <= 1; cX += 2)
		{
			if ((cX == -1 && currentNode->x + cX != -1) || (cX == 1 && currentNode->x + cX != width))
			{
				Node* newNode = new Node();
				newNode->x = currentNode->x + cX;
				newNode->y = currentNode->y;
				addNodeToOpenSet(closedSet, newNode, openSet, currentNode);
			}
		}

		//Check the nodes above and below the current node if in range
		for (int cY = -1; cY <= 1; cY += 2)
		{
			if ((cY == -1 && currentNode->y + cY != -1) || (cY == 1 && currentNode->y + cY != height))
			{
				Node* newNode = new Node();
				newNode->x = currentNode->x;
				newNode->y = currentNode->y + cY;
				addNodeToOpenSet(closedSet, newNode, openSet, currentNode);
			}
		}
	}//End while

	cout << "Open Set Size: " << openSet.size() << endl;
	cout << "Closed Set Size: " << closedSet.size() << endl;

	//Clear the open and closed sets, since they are no longer needed
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
	}//End if
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
	else cout << "NOT FOUND" << endl;
}