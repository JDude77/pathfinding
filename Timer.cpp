#include "Timer.h"

void Timer::startTimer()
{
	if (!hasStarted)
	{
		resetTimer();
		startTime = steady_clock::now();
		hasStarted = true;
	}
	else
	{
		std::cout << "ERROR: Attempt to start timer when already running." << std::endl;
	}
}

void Timer::stopTimer()
{
	if (hasStarted)
	{
		endTime = steady_clock::now();
		savedTime = duration_cast<nanoseconds>(endTime - startTime).count();
		hasStarted = false;
	}
	else
	{
		std::cout << "ERROR: Attempt to stop timer when not running." << std::endl;
	}
}

_int64 Timer::getSavedTime()
{
	if (savedTime != NULL)
	{
		return savedTime;
	}

	else
	{
		std::cout << "ERROR: Attempt to retrieve null time." << std::endl;
		return NULL;
	}
}

void Timer::resetTimer()
{
	savedTime = NULL;
	hasStarted = false;
}
