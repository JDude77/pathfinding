#pragma once
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::steady_clock;

class Timer
{
	private:
		bool hasStarted = false;
		_int64 savedTime;
		steady_clock::time_point startTime;
		steady_clock::time_point endTime;

	public:
		void startTimer();
		void stopTimer();
		_int64 getSavedTime();
		void resetTimer();
};

