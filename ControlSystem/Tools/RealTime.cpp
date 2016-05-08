#include "RealTime.h"
#include <unistd.h>		// for sleep and usleep
#include <time.h>		// for nanosleep

using namespace R2D2;

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
