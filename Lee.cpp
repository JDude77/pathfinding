#include "Lee.h"

bool Lee::floodGridLee(int grid[maxHeight][maxWidth], Node* start, Node* end, int width, int height)
{
	int distance = 0;
	bool stuck = false, change = false;

	grid[start->y][start->x] = distance;

	while (grid[end->y][end->x] == -1 && !stuck)
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
					if (x < width - 1)
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
					if (y < height - 1)
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

void Lee::findPath(int grid[maxHeight][maxWidth], Node* start, Node* end, int width, int height)
{
	path.clear();
	bool pathFound = floodGridLee(grid, start, end, width, height);

	if (pathFound)
	{
		Node* currentPos = end;
		int distance = grid[end->y][end->x];
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

		//Reverse the path so it goes from start to finish, not finish to start
		path.reverse();
	}//End if

	//If a path has not been found, throw an in-program error
	else
	{
		cout << "No Valid Path Found" << endl;
	}//End else
}

void Lee::destroyPath(Node* start, Node* end)
{
	//Clear the path
	for (Node* currentNode : path)
	{
		if (currentNode != start && currentNode != end)
		{
			delete currentNode;
		}//End if
	}//End for
	path.clear();
}

void Lee::printPath()
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

list<Node*> Lee::getPath()
{
	return path;
}
