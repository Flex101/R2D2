#include "FootDrive.h"

#include <string.h>					// for memset()
#include <stdio.h>					// for sprintf()
#include "SerialDeviceLibrary.h"
#include "Logging.h"
#include "math.h"

using namespace R2D2;
using namespace R2D2::Devices;

FootDrive::FootDrive(std::string _ID)
{
	ID = _ID;
	speed = 0;

	ccCmd[0] = 'C';
	ccCmd[1] = 'C';
	ccCmd[2] = 13;

	ssCmd[0] = 'S';
	ssCmd[1] = 'S';
	ssCmd[7] = 13;

	setReversed(false);
	setMaxSpeed(1.0f);
	setSpeed(0.0f);
}

FootDrive::~FootDrive()
{
}

bool FootDrive::connect()
{
	bool success = SerialDevice::connect();
	success &= commsCheck();

	if (success) Logging::log(LOG_INFO, ID, "Connected");
	else Logging::log(LOG_ERROR, ID, "Failed to connect");

	return success;
}

bool FootDrive::reconnect()
{
	bool success = SerialDevice::connect();
	success &= commsCheck();

	if (success) Logging::log(LOG_INFO, ID, "Reconnected");
	else SerialDevice::disconnect();

	return success;
}

void FootDrive::disconnect()
{
	if (!connected) return;
	speed = 0;
	writeSpeed();
	SerialDevice::disconnect();
	Logging::log(LOG_INFO, ID, "Disconnected");
}

void FootDrive::poll()
{
	if (!connected) return;

	writeSpeed();
	memset(&reply, 0, sizeof(reply));
	readLine((byte*)&reply);
	replyStr = (char*)&reply;

	if (replyStr.empty())
	{
		Logging::log(LOG_WARN, ID, "No reply! Assuming disconnected.", replyStr.c_str());
		disconnect();
	}
	else if (replyStr != "SSOK")
	{
		 Logging::log(LOG_WARN, ID, "Irregular reply: %s", replyStr.c_str());
	}
}

void FootDrive::initialise(SerialDeviceLibrary& library)
{
	std::string path = library.pathFromID(ID);

	if (path.empty())
	{
		Logging::log(LOG_ERROR, ID, "Unable to find device");
		return;
	}

	SerialDevice::initialise(path, B115200);
}

void FootDrive::setReversed(bool _reversed)
{
	reversed = _reversed;
}

void FootDrive::setMaxSpeed(float value)
{
	if (value > 1) value = 1;
	if (value < 0) value = 0;
	maxSpeed = value;
}

void FootDrive::setSpeed(float value)
{
	if (value < -1) value = -1;
	if (value > 1) value = 1;

	if (reversed) speed = -value * maxSpeed;
	else speed = value * maxSpeed;

	char buff[4];
	sprintf(buff, "%d", (int)(fabs(speed) * 1024));
	std::string ssStr = buff;

	if (speed >= 0) ssCmd[2] = '+';
	else ssCmd[2] = '-';

	for (int i = 0; i < 4; ++i)
	{
		if (i < (int)ssStr.length()) ssCmd[6-i] = ssStr.at(ssStr.length() - (i + 1));
		else ssCmd[6-i] = '0';
	}
}

bool FootDrive::commsCheck(unsigned int retryLimit)
{
	for (unsigned int i = 0; i < retryLimit; ++i)
	{
		writeData((byte*)&ccCmd, 3);
		memset(&reply, 0, sizeof(reply));
		readLine((byte*)&reply, 50, true);
		replyStr = (char*)&reply;

		if (replyStr == "CCOK")
		{
			SerialDevice::flush();
			return true;
		}
	}

	return false;
}

void FootDrive::writeSpeed()
{
	writeData((byte*)&ssCmd, 8);
}
