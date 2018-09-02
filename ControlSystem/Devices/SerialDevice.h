#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include "Devices/Device.h"
#include "Tools/RealTime.h"		// for Timer;
#include <termios.h>			// for POSIX terminal control definitions
#include <string>

namespace R2D2 {

typedef unsigned char byte;

namespace Devices {

class SerialDevice : public Device
{
public:
	explicit SerialDevice() {}
	virtual ~SerialDevice();

	void initialise(std::string path, unsigned int baudrate);

	virtual bool connect();
	virtual void disconnect();
	virtual bool poll();

	bool writeData(byte* data, unsigned int length);
	int readData(byte* data);
	int readLine(byte* data, unsigned int timeoutMillisecs = 10, bool silent = false);
	void flush();

protected:
	std::string path;
	unsigned int baudrate;
	int fd;
	struct termios tty;
	Timer timeoutTimer;
};

} // namespace Devices
} // namespace R2D2

#endif // SERIALDEVICE_H
