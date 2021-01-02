#include "Grid.h"
#include "Timer.h"
#include <Windows.h>
#include <fstream>

using std::ofstream;

int main(int argc, char* argv[])
{
	Grid g;
	Timer timer;
	_int64 totalAStar = 0, totalLee = 0;
	float avgTimeAStar = 0, avgTimeLee = 0;
	int pathLengthAStar = 0, pathLengthLee = 0;
	float percentTrack = 0;
	float progress = 0.0f;

	ofstream testTimes("Random (SIZE 10 OBSTACLE CHANCE 0).csv");

	for (int i = 0; i < 1000; i++)
	{		
		avgTimeAStar = 0; avgTimeLee = 0;
		pathLengthAStar = 0; pathLengthLee = 0;

		g.initialiseRandomGrid();

		//Run through each grid five times for more accurate values
		for (int run = 0; run < 5; run++)
		{
			//cout << "A*" << endl;
			timer.startTimer();
			g.findPathAStar();
			timer.stopTimer();
			totalAStar += timer.getSavedTime();
			avgTimeAStar += timer.getSavedTime();
			pathLengthAStar = g.getPath().size();
			//g.printPath();
			//cout << "Time " << percentTrack + 1 << ": " << timer.getSavedTime() << " nanoseconds" << endl << endl;

			g.resetGrid();

			//cout << "Lee" << endl;
			timer.startTimer();
			g.findPathLee();
			timer.stopTimer();
			totalLee += timer.getSavedTime();
			avgTimeLee += timer.getSavedTime();
			pathLengthLee = g.getPath().size();
			//g.printPath();
			percentTrack += 1;
			//cout << "Time " << percentTrack << ": " << timer.getSavedTime() << " nanoseconds" << endl << endl;

			g.resetGrid();
			progress = percentTrack / 5000.0 * 100.0;
			//cout << progress << "%" << endl;
		}

		avgTimeAStar /= 5;
		avgTimeLee /= 5;

		//Add A Star times to CSV
		testTimes << pathLengthAStar << "," << avgTimeAStar << ",";
 
		//Add Lee times to CSV
		testTimes << pathLengthLee << "," << avgTimeLee << endl;
	}

	cout << "Average Time Taken (A*): " << totalAStar / 5000 << " nanoseconds" << endl;
	cout << "Average Time Taken (Lee): " << totalLee / 5000 << " nanoseconds" << endl;
	return 0;
}