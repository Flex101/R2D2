#ifndef TIME_H
#define TIME_H

#include <time.h>		// for timespec and nanosleep()

namespace R2D2 {

/***** RealTime *****/

class RealTime
{
public:
	static void sleepNano(unsigned int nanoSecs);
	static void sleepMicro(unsigned int microSecs);
	static void sleepMilli(unsigned int milliSecs);
	static void sleepSec(unsigned int seconds);

	static timespec timeDiff(const timespec& start, const timespec& end);
	static bool isAfter(const timespec& time1, const timespec& time2);
};

/***** StopWatch *****/

class StopWatch
{
public:
	explicit StopWatch();
	virtual ~StopWatch() {}

	void start();
	void stop();
	void restart();

	timespec elapsed();

protected:
	bool running;
	timespec startTime;
	timespec now;
};

/***** Timer *****/

class Timer
{
public:
	explicit Timer();
	virtual ~Timer() {}

	void start(timespec duration);
	void stop();
	void restart();

	void startSec(unsigned int seconds);
	void startMilli(unsigned int milliSecs);
	void startMicro(unsigned int microSecs);
	void startNano(unsigned int nanoSecs);

	bool hasElapsed();

protected:
	StopWatch stopwatch;
	timespec duration;
};


} // namespace R2D2

#endif // TIME_H
