#ifndef PS3CONTROLLER_H
#define PS3CONTROLLER_H

#include "Devices/R2Controller.h"
#include "Devices/Gamepad.h"
#include "Tools/DeviceInfo.h"

#endif // PS3CONTROLLER_H

namespace R2D2 {
namespace Devices {

class PS3Controller : public R2Controller, public Gamepad
{
public:
	explicit PS3Controller(std::string mac);
	virtual ~PS3Controller() {}

	virtual bool connect();
	virtual void disconnect();

	virtual bool poll();

	/*** FRIENDLY MAPPINGS ***/

	float joyLeftX()			{ return getAxisValue(0); }
	float joyLeftY()			{ return getAxisValue(1); }
	float joyRightX()			{ return getAxisValue(2); }
	float joyRightY()			{ return getAxisValue(3); }

	bool joyLeftPress()			{ return getButtonValue(10); }
	bool joyRightPress()		{ return getButtonValue(11); }

	bool dPadLeft()				{ return (getAxisValue(4) < 0); }
	bool dPadRight()			{ return (getAxisValue(4) > 0); }
	bool dPadUp()				{ return (getAxisValue(5) < 0); }
	bool dPadDown()				{ return (getAxisValue(5) > 0); }

	bool square()				{ return getButtonValue(3); }
	bool circle()				{ return getButtonValue(1); }
	bool triangle()				{ return getButtonValue(2); }
	bool cross()				{ return getButtonValue(0); }

	bool shoulderLeftTop()		{ return getButtonValue(4); }
	bool shoulderLeftBottom()	{ return getButtonValue(6);}
	bool shoulderRightTop()		{ return getButtonValue(5); }
	bool shoulderRightBottom()	{ return getButtonValue(7); }

	bool select()				{ return getButtonValue(8); }
	bool start()				{ return getButtonValue(9); }

	/*** R2Controller Functions ***/

	virtual bool isConnected()			{ return Device::isConnected(); }
	virtual float feetjoyX()			{ return joyLeftX(); }
	virtual float feetJoyY()			{ return joyLeftY(); }
	virtual float domeJoyX()			{ return joyRightX(); }
	virtual float domeJoyY()			{ return joyRightY(); }
	virtual bool deadMan()				{ return shoulderLeftTop(); }
	virtual bool highSpeedEnable()		{ return shoulderLeftBottom(); }
	virtual bool positiveSound()		{ return triangle(); }
	virtual bool neutralSound()			{ return circle(); }
	virtual bool negativeSound()		{ return cross(); }

protected:
	Tools::DeviceInfoReader devInfoReader;
};

} // namespace Devices
} // namespace R2D2
