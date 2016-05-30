#include "RealTime.h"
#include <unistd.h>		// for sleep and usleep

using namespace R2D2;

/***** RealTime *****/

void RealTime::sleepNano(unsigned int nanoSecs)
{
	struct timespec spec;
	spec.tv_sec = nanoSecs / 1000000000;
	spec.tv_nsec = nanoSecs % 1000000000;

	nanosleep(&spec, NULL);
}

void RealTime::sleepMicro(unsigned int microSecs)
{
	usleep(microSecs);
}

void RealTime::sleepMilli(unsigned int milliSecs)
{
	usleep(milliSecs * 1000);
}

void RealTime::sleepSec(unsigned int seconds)
{
	sleep(seconds);
}

timespec RealTime::timeDiff(const timespec& start, const timespec& end)
{
	timespec diff;

	if ((end.tv_nsec - start.tv_nsec) < 0)
	{
		diff.tv_sec = end.tv_sec - start.tv_sec - 1;
		diff.tv_nsec = end.tv_nsec - start.tv_nsec + 1000000000;
	}
	else
	{
		diff.tv_sec = end.tv_sec - start.tv_sec;
		diff.tv_nsec = end.tv_nsec - start.tv_nsec;
	}

	return diff;
}

bool RealTime::isAfter(const timespec& time1, const timespec& time2)
{
	if (time1.tv_sec > time2.tv_sec) return true;
	if (time1.tv_sec < time2.tv_sec) return false;
	if (time1.tv_nsec > time2.tv_nsec) return true;

	return false;
}

/***** StopWatch *****/

StopWatch::StopWatch()
{
	running = false;
}

void StopWatch::start()
{
	clock_gettime(CLOCK_REALTIME, &startTime);
	running = true;
}

void StopWatch::stop()
{
	running = false;
}

void StopWatch::restart()
{
	clock_gettime(CLOCK_REALTIME, &startTime);
}

timespec StopWatch::elapsed()
{
	if (!running)
	{
		now.tv_sec = 0;
		now.tv_nsec = 0;
		return now;
	}

	clock_gettime(CLOCK_REALTIME, &now);
	return RealTime::timeDiff(startTime, now);
}

/***** Timer *****/

Timer::Timer()
{

}

void Timer::start(timespec _duration)
{
	duration = _duration;
	stopwatch.start();
}

void Timer::stop()
{
	stopwatch.stop();
}

void Timer::restart()
{
	stopwatch.restart();
}

void Timer::startSec(unsigned int seconds)
{
	duration.tv_sec = seconds;
	duration.tv_nsec = 0;
	stopwatch.start();
}

void Timer::startMilli(unsigned int milliSecs)
{
	duration.tv_sec = 0;
	duration.tv_nsec = milliSecs * 1000000;
	stopwatch.start();
}

void Timer::startMicro(unsigned int microSecs)
{
	duration.tv_sec = 0;
	duration.tv_nsec = microSecs * 1000;
	stopwatch.start();
}

void Timer::startNano(unsigned int nanoSecs)
{
	duration.tv_sec = 0;
	duration.tv_nsec = nanoSecs;
	stopwatch.start();
}

bool Timer::hasElapsed()
{
	return RealTime::isAfter(stopwatch.elapsed(), duration);
}
