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

	grid.initialiseRandomGrid();
	grid.displayGrid();

	grid.pathMode = grid.LEE;
	timer.startTimer();
		grid.findPath();
	timer.stopTimer();
	grid.displayGrid();

	cout << timer.getSavedTime() << endl;
	cout << "Length: " << grid.getPath().size() << "\n";
	grid.printPath();
	grid.cleanUp();

	grid.pathMode = grid.ASTAR;
	timer.startTimer();
		grid.findPath();
	timer.stopTimer();
	grid.displayGrid();

	cout << timer.getSavedTime() << endl;
	cout << "Length: " << grid.getPath().size() << "\n";
	grid.printPath();
	grid.cleanUp();

	return 0;
}