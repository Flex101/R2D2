#include "Gamepad.h"

#include "Tools/File.h"
#include "Tools/Logging.h"
#include <fcntl.h>				// for open()
#include <unistd.h>				// for read() and close()
#include <errno.h>				// for catching EAGAIN
#include <sys/ioctl.h>			// for ioctl()
#include <linux/joystick.h>		// for JSIOCGAXES and JSIOCGBUTTONS
#include <linux/input.h>		// for vibrate
#include <string.h>				// for memset

using namespace R2D2;
using namespace R2D2::Devices;

Gamepad::Gamepad(std::string name)
{
	inputLocation = "/dev/input/by-id/usb-" + name + "-joystick";
	outputLocation = "/dev/input/by-id/usb-" + name + "-event-joystick";
	reset();
}

Gamepad::~Gamepad()
{
}

bool Gamepad::connect(bool withFF)
{
	if (connected) return true;

	input_id = open(inputLocation.c_str(), O_NONBLOCK);
	if (withFF) output_id = open(outputLocation.c_str(), O_RDWR);

	connected = (input_id > 0);
	if (withFF) connected &= (output_id > 0);

	if (connected)
	{
		if (withFF) Logging::log(LOG_INFO, "GAMEPAD" ,"Connected (with rumble)");
		else Logging::log(LOG_INFO, "GAMEPAD", "Connected");
		initialise();
	}
	else
	{
		Logging::log(LOG_WARN, "GAMEPAD", "Failed to connect");
	}

	return connected;
}

void Gamepad::disconnect()
{
	if (!connected) return;

	if (input_id > 0) close(input_id);
	if (output_id > 0) close(output_id);

	reset();
	Logging::log(LOG_INFO, "GAMEPAD", "Disconnected");
}

void Gamepad::poll()
{
	if (!File::fileExists(inputLocation))
	{
		close(input_id);
		reset();
		Logging::log(LOG_WARN, "GAMEPAD", "Lost connection");
		return;
	}

	while(readEvent())
	{
		//inputEvent.type &= ~JoyEventType_Init;	// Commented out so the initial state is assumed all zeros - this is safest

		if (inputEvent.type == JoyEventType_Axis)
		{
			axisValues.at(inputEvent.index) = ((float)inputEvent.value / 32767);
			Logging::log(LOG_DEBUG, "GAMEPAD", "Axis: %d %d, %f", inputEvent.index, inputEvent.value, axisValues.at(inputEvent.index));
		}
		else if (inputEvent.type == JoyEventType_Button)
		{
			buttonValues.at(inputEvent.index) = (inputEvent.value > 0);
			Logging::log(LOG_DEBUG, "GAMEPAD", "Button: %d %d", inputEvent.index, inputEvent.value);
		}
	}
}

float Gamepad::getAxisValue(unsigned int index)
{
	if (index >= axisValues.size()) return 0;
	else return axisValues.at(index);
}

bool Gamepad::getButtonValue(unsigned int index)
{
	if (index >= buttonValues.size()) return 0;
	else return buttonValues.at(index);
}

void Gamepad::initialise()
{
	char axisCount;
	ioctl(input_id, JSIOCGAXES, &axisCount);
	axisValues.resize(axisCount, 0);
	Logging::log(LOG_DEBUG, "GAMEPAD", "Axis Count: %d", axisCount);

	char buttonCount;
	ioctl(input_id, JSIOCGBUTTONS, &buttonCount);
	buttonValues.resize(buttonCount, false);
	Logging::log(LOG_DEBUG, "GAMEPAD", "Button Count: %d", buttonCount);
}

void Gamepad::reset()
{
	connected = false;
	input_id = -1;
	output_id = -1;
	axisValues.clear();
	buttonValues.clear();
}

bool Gamepad::readEvent()
{
	int bytes = read(input_id, &inputEvent, sizeof(JoyInputEvent));

	if (bytes < 0) return false;
	if (bytes == sizeof(JoyInputEvent)) return true;

	Logging::log(LOG_WARN, "GAMEPAD Invalid event packet");

	return false;
}
