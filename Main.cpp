#include "Grid.h"
#include "Timer.h"

int main(int argc, char* argv[])
{
	Grid g;
	Timer timer;

	for (int i = 0; i < 10; i++)
	{
		g.initialiseGrid();
		timer.startTimer();
		g.floodGridLee();
		g.createPathLee();
		//g.findPathAStar();
		timer.stopTimer();
		g.displayGrid();
		g.printPath();
		cout << "Time: " << timer.getSavedTime() << endl;
	}
	return 0;
}