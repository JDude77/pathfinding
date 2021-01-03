#include "Grid.h"
#include <Windows.h>
#include <chrono>

using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::high_resolution_clock;

void Grid::displayGrid()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < width + 2; i++)
	{
		cout << "-";
	}//End for
	cout << endl;

	for (int i = 0; i < height; i++)
	{
		SetConsoleTextAttribute(hConsole, 7);
		cout << "|";
		for (int j = 0; j < width; j++)
		{
			bool nodeInPath = false;
			//Print special values for nodes on the path
			if (!path.empty())
			{
				for (Node* node : path)
				{
					if ((node->x == j && node->y == i))
					{
						if (start->x == j && start->y == i)
						{
							SetConsoleTextAttribute(hConsole, 170);
							cout << "█";
						}//End if
						else if (end->x == j && end->y == i)
						{
							SetConsoleTextAttribute(hConsole, 68);
							cout << "█";
						}//End else if
						else
						{
							SetConsoleTextAttribute(hConsole, 153);
							cout << "█";
						}//End else
						nodeInPath = true;
						break;
					}//End if
				}//End for
				if (!nodeInPath)
				{
					if (start->x == j && start->y == i)
					{
						SetConsoleTextAttribute(hConsole, 170);
						cout << "█";
					}//End if
					else if (end->x == j && end->y == i)
					{
						SetConsoleTextAttribute(hConsole, 68);
						cout << "█";
					}//End else if
					switch (grid[i][j])
					{
						//Obstacle
						case -2:
							SetConsoleTextAttribute(hConsole, 255);
							cout << "█";
							break;
						//Non-Obstacle But Not Checked
						case -1:
							SetConsoleTextAttribute(hConsole, 119);
							cout << "█";
							break;
						//Checked
						default:
							SetConsoleTextAttribute(hConsole, 136);
							cout << "█";
							break;
					}//End switch
				}//End if
			}//End if
			else
			{
				if (start->x == j && start->y == i)
				{
					SetConsoleTextAttribute(hConsole, 170);
					cout << "█";
				}//End if
				else if (end->x == j && end->y == i)
				{
					SetConsoleTextAttribute(hConsole, 68);
					cout << "█";
				}//End else if
				else
				{
					switch (grid[i][j])
					{
						//Obstacle
					case -2:
						SetConsoleTextAttribute(hConsole, 255);
						cout << "█";
						break;
						//Non-Obstacle But Not Checked
					case -1:
						SetConsoleTextAttribute(hConsole, 119);
						cout << "█";
						break;
						//Checked
					default:
						SetConsoleTextAttribute(hConsole, 136);
						cout << "█";
						break;
					}//End switch
				}//End else
			}//End else
		}//End height for
		SetConsoleTextAttribute(hConsole, 7);
		cout << "|";
		//New line
		cout << "\n";
	}//End width for
	for (int i = 0; i < width + 2; i++)
	{
		cout << "-";
	}//End for
	cout << "\n";
	cout << "START:\t" << start->x << ", " << start->y << endl;
	cout << "END:\t" << end->x << ", " << end->y << endl;
}

void Grid::initialiseRandomGrid()
{
	auto time = std::chrono::high_resolution_clock::now();
	auto timeToNanoForSeed = duration_cast<nanoseconds>(time.time_since_epoch());

	srand((unsigned int)timeToNanoForSeed.count());

	//Random start and end place
	start = new Node();
	start->x = rand() % width;
	start->y = rand() % height;

	end = new Node();
	end->x = rand() % width;
	end->y = rand() % height;

	//To get more varied start and end points (and guarantee they're not occupying the same spot)
	if (start->x == end->x && start->y == end->y)
	{
		switch (rand() % 2)
		{
			case 1: while (start->x == end->x) end->x = rand() % width; break;
			default: while (start->y == end->y) end->y = rand() % height; break;
		}//End switch
	}//End if

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (rand() % 100 < obstacleChance)
			{
				grid[j][i] = -2;
			}//End if
			else
			{
				grid[j][i] = -1;
			}//End else
		}//End j for
	}//End i for

	//Set the display value for the start node to a distance of 0
	grid[start->y][start->x] = 0;

	//Make sure that the end value isn't an unreachable obstacle
	grid[end->y][end->x] = -1;
}

list<Node*> Grid::getPath()
{
	return path;
}

void Grid::findPath()
{
	switch (pathMode)
	{
		case ASTAR:
			aStar.findPath(grid, height, width, start, end);
			path = aStar.getPath();
			break;
		case LEE:
			lee.findPath(grid, start, end, width, height);
			path = lee.getPath();
			break;
	}//End switch
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

void Grid::cleanUp()
{
	//Clear the grid content
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (grid[j][i] == -2) continue;

			else
			{
				grid[j][i] = -1;
			}//End else
		}//End j for
	}//End i for

	//Set the display value for the start node to a distance of 0
	grid[start->y][start->x] = 0;

	destroyPath();

	switch (pathMode)
	{
		case LEE:
			lee.getPath().clear();
			break;
		case ASTAR:
			aStar.getPath().clear();
			break;
	}//End switch
}

void Grid::destroyPath()
{
	//Clear the path
	for (Node* currentNode : path)
	{
		if (currentNode != end && currentNode != start)
		{
			delete currentNode;
		}//End if
	}//End for
	path.clear();
}


Grid::~Grid()
{
	destroyPath();
	delete start;
	delete end;
}