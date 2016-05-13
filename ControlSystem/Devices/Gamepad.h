#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "Device.h"
#include "CommonDefs.h"
#include <string>
#include <vector>

namespace R2D2 {
namespace Devices {

class Gamepad : public Device
{

public:
	explicit Gamepad(std::string name);
	virtual ~Gamepad();

	virtual bool connect(bool withFF = false);
	virtual void disconnect();

	virtual void poll();

	float getAxisValue(unsigned int index);
	bool getButtonValue(unsigned int index);

protected:
	virtual void initialise();
	void reset();

	bool readEvent();

protected:

	struct JoyInputEvent
	{
		uint32_t time;
		int16_t value;
		uint8_t type;
		uint8_t index;
	};

	struct JoyOutputEvent
	{
		struct timeval time;
		uint16_t type;
		uint16_t code;
		int32_t value;
	};

	enum JoyEventType
	{
		JoyEventType_Button =	0x01,
		JoyEventType_Axis =		0x02,
		JoyEventType_Init =		0x80
	};

	std::string inputLocation;
	int input_id;
	struct JoyInputEvent inputEvent;

	std::string outputLocation;
	int output_id;
	struct JoyOutputEvent outputEvent;

	std::vector<float> axisValues;
	std::vector<bool> buttonValues;
};

} // namespace Devices
} // namespace R2D2

#endif // GAMEPAD_H