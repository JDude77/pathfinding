#include "Grid.h"
#include "Timer.h"
#include <Windows.h>
#include <fstream>
#include <list>

using std::ofstream;

int main(int argc, char* argv[])
{
	Grid grid;
	Timer timer;
	bool end = false;
	while (!end)
	{

		//Allow user to choose which algorithm to run (or both)
		cout << "Welcome to the pathfinding program. Which algorithm would you like to test out? Enter a number:" << endl;
		cout << "1: Lee's\n2: A*\n3: Both\n\n> ";

		char algorithmChoice = ' ';
		cin >> algorithmChoice;

		while (algorithmChoice != '1' && algorithmChoice != '2' && algorithmChoice != '3')
		{
			cout << "ERROR: Invalid input. Please choose which algorithm you wish to run using the numbers provided:" << endl;
			cout << "1: Lee's\n2: A*\n3: Both\n\n> ";
			cin >> algorithmChoice;
		}//End while

		//Allow user to choose obstacle spawn chance
		cout << "Please enter the % chance of an obstacle spawning you want for this grid (non-valid entries will default to 0%)." << endl << endl << "> ";

		int obstacleChance;
		cin >> obstacleChance;
		grid.setObstacleChance(obstacleChance);

		grid.initialiseRandomGrid();
		grid.displayGrid();

		//Lee Running
		if (algorithmChoice == '1' || algorithmChoice == '3')
		{
			grid.pathMode = grid.LEE;
			timer.startTimer();
				grid.findPath();
			timer.stopTimer();
			grid.displayGrid();

			cout << timer.getSavedTime() << endl;
			cout << "Length: " << grid.getPath().size() << "\n";
			grid.printPath();
			grid.cleanUp();
		}//End if

		//A Star Running
		if (algorithmChoice == '2' || algorithmChoice == '3')
		{
			grid.pathMode = grid.ASTAR;
			timer.startTimer();
				grid.findPath();
			timer.stopTimer();
			grid.displayGrid();

			cout << timer.getSavedTime() << endl;
			cout << "Length: " << grid.getPath().size() << "\n";
			grid.printPath();
			grid.cleanUp();
		}//End if

		cout << "PATHFINDING COMPLETE" << endl;
		cout << "Would you like to run this program again? Y/N:\n\n> ";
		char again = ' ';
		cin >> again;
		while (again != 'Y' && again != 'y' && again != 'N' && again != 'n')
		{
			cout << "ERROR: Please respond with Y or N. Do you want to run this program again?:\n\n> ";
			cin >> again;
		}//End while

		if (again == 'N' || again == 'n') end = true;
	}//End while

	return 0;
}