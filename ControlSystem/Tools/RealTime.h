#ifndef TIME_H
#define TIME_H

namespace R2D2 {

class RealTime
{
public:
	static void sleepNano(unsigned int nanoSecs);
	static void sleepMicro(unsigned int microSecs);
	static void sleepMilli(unsigned int milliSecs);
	static void sleepSec(unsigned int seconds);
};

} // namespace R2D2

#endif // TIME_H
