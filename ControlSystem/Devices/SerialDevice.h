#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include "Device.h"
#include "RealTime.h"	// for Timer;
#include <termios.h>	// for POSIX terminal control definitions
#include <string>

namespace R2D2 {

typedef unsigned char byte;

namespace Devices {

class SerialDevice : public Device
{
public:
	explicit SerialDevice();
	virtual ~SerialDevice();

	void initialise(std::string path, unsigned int baudrate);

	virtual bool connect();
	virtual void disconnect();
	virtual void poll();

	bool writeData(byte* data, unsigned int length);
	int readData(byte* data);
	int readLine(byte* data);

protected:
	std::string path;
	unsigned int baudrate;
	int fd;
	struct termios tty;
	Timer timeout;
};

} // namespace Devices
} // namespace R2D2

#endif // SERIALDEVICE_H
