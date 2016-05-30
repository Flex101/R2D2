#include "SerialDevice.h"

#include <fcntl.h>				// for open()
#include <unistd.h>				// for read(), write() and close()
#include <string.h>				// for memset()
#include "Logging.h"

using namespace R2D2;
using namespace R2D2::Devices;

SerialDevice::SerialDevice()
{

}

SerialDevice::~SerialDevice()
{
	if (connected) disconnect();
}

void SerialDevice::initialise(std::string _path, unsigned int _baudrate)
{
	path = _path;
	baudrate = _baudrate;
	fd = -1;
}

bool SerialDevice::connect()
{
	if (path.empty()) return false;
	fd = open(path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd < 0) return false;

	memset (&tty, 0, sizeof tty);
	cfsetospeed (&tty, (speed_t)baudrate);
	cfsetispeed (&tty, (speed_t)baudrate);
	tty.c_cflag     &=  ~PARENB;							// Make 8n1
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;
	tty.c_cflag     &=  ~CRTSCTS;							// no flow control
	tty.c_lflag     =   0;									// no signaling chars, no echo, no canonical processing
	tty.c_oflag     =   0;									// no remapping, no delays
	tty.c_cc[VMIN]      =   0;								// read doesn't block
	tty.c_cc[VTIME]     =   5;								// 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL;						// turn on READ & ignore ctrl lines
	tty.c_iflag     &=  ~(IXON | IXOFF | IXANY);			// turn off s/w flow ctrl
	tty.c_lflag     &=  ~(ICANON | ECHO | ECHOE | ISIG);	// make raw
	tty.c_oflag     &=  ~OPOST;								// make raw

	tcflush(fd,TCIFLUSH);

	if (tcsetattr(fd,TCSANOW,&tty) != 0)
	{
		close(fd);
		return false;
	}

	connected = true;
	return true;
}

void SerialDevice::disconnect()
{
	if (!connected) return;
	close(fd);
	connected = false;
}

void SerialDevice::poll()
{

}

bool SerialDevice::writeData(byte* data, unsigned int length)
{
	if (!connected) return false;
	int bytesWritten = write(fd, data, length);
	return (bytesWritten == (int)length);
}

int SerialDevice::readData(byte* data)
{
	if (!connected) return -1;

	byte buffer = 0;
	int bytesRead = 0;

	while (true)
	{
		if (read(fd, &buffer, 1 ) > 0)
		{
			*data = buffer;
			data++;
			bytesRead++;
		}
		else break;
	}

	return bytesRead;
}

int SerialDevice::readLine(byte* data, unsigned int timeoutMillisecs)
{
	if (!connected) return -1;

	byte buffer = 0;
	int bytesRead = 0;

	timeoutTimer.startMilli(timeoutMillisecs);

	while (true)
	{
		if (read(fd, &buffer, 1 ) > 0)
		{
			bytesRead++;
			if (buffer == '\r') break;
			*data = buffer;
			data++;
		}

		if (timeoutTimer.hasElapsed())
		{
			Logging::log(LOG_WARN, "SERIAL", "readLine() timeout");
			break;
		}
	}

	return bytesRead;
}
