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

	ssCmd[0] = 'S';
	ssCmd[1] = 'S';
	ssCmd[7] = 13;

	reversed = false;
	setSpeed(0.0f);
}

FootDrive::~FootDrive()
{
}

bool FootDrive::connect()
{
	bool success = SerialDevice::connect();

	if (success) Logging::log(LOG_INFO, ID, "Connected");
	else Logging::log(LOG_ERROR, ID, "Failed to connect");

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
	memset(&ssReply, 0, sizeof(ssReply));
	readLine((byte*)&ssReply);
	replyStr = (char*)&ssReply;
	if (replyStr != "SSOK") Logging::log(LOG_WARN, ID, "Irregular reply: %s", replyStr.c_str());
}

void FootDrive::initialise(SerialDeviceLibrary& library)
{
	std::string path = library.pathFromID(ID);

	if (path.empty())
	{
		Logging::log(LOG_ERROR, ID, "Unable to find foot drive path");
		return;
	}

	SerialDevice::initialise(path, B115200);
}

void FootDrive::setReversed(bool _reversed)
{
	reversed = _reversed;
}

void FootDrive::setSpeed(float value)
{
	if (value < -1) value = -1;
	if (value > 1) value = 1;

	if (reversed) speed = -value;
	else speed = value;

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

void FootDrive::writeSpeed()
{
	writeData((byte*)&ssCmd, 8);
}
