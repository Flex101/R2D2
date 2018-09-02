#ifndef R2CONTROLLER_H
#define R2CONTROLLER_H

#include "Devices/Device.h"

namespace R2D2 {
namespace Devices {

class R2Controller : public Device
{
public:
	// Connection
	virtual bool isConnected() = 0;

	// Feet joystick
	virtual float feetjoyX() = 0;
	virtual float feetJoyY() = 0;

	// Dome joystick
	virtual float domeJoyX() = 0;
	virtual float domeJoyY() = 0;

	// Dead-man switch
	virtual bool deadMan() = 0;

	// High speed enabled
	virtual bool highSpeedEnable() = 0;

	// Audio triggers
	virtual bool positiveSound() = 0;
	virtual bool neutralSound() = 0;
	virtual bool negativeSound() = 0;
};

} // namespace Devices
} // namespace R2D2

#endif // R2CONTROLLER_H
