#include "Devices/PS3Controller.h"

#include "Tools/Logging.h"

using namespace R2D2;
using namespace R2D2::Devices;

unsigned int PS3Controller::buttonRemap[17] = {14, 13, 12, 15, 10, 11, 8, 9, 0, 3, 16, 1, 2, 4, 6, 7, 5};

PS3Controller::PS3Controller(std::string mac) : Gamepad()
{
	std::string devicePath = devInfoReader.joystickFromMac(mac);

	if (!devicePath.empty())
	{
		this->setDevicePath(devicePath);
		Logging::log(LOG_INFO, "PS3CONTROLLER", "Found device = %s", devicePath.c_str());
	}
	else
	{
		Logging::log(LOG_ERROR, "PS3CONTROLLER", "Unable to find device");
	}
}

bool PS3Controller::connect()
{
	if (connected) return true;

	connected = Gamepad::connect();
	return connected;
}

void PS3Controller::disconnect()
{
	if (!connected) return;

	Gamepad::disconnect();
	connected = false;
}

bool PS3Controller::poll()
{
	bool success = Gamepad::poll();
	if (!success) disconnect();
	return success;
}

bool PS3Controller::getButtonValue(unsigned int index)
{
	if (axisValues.size() > 6)	// Requires buttonRemap
	{
		return Gamepad::getButtonValue(buttonRemap[index]);
	}
	else return Gamepad::getButtonValue(index);
}
