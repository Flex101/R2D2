#ifndef LOGITECHCRP2_H
#define LOGITECHCRP2_H

#include "Devices/R2Controller.h"
#include "Devices/Gamepad.h"

struct ff_effect;

namespace R2D2 {
namespace Devices {

class LogitechCRP2 : public R2Controller, public Gamepad
{
public:
	explicit LogitechCRP2();
	virtual ~LogitechCRP2();

	virtual bool connect();
	virtual void disconnect();

	virtual void initialise();
	virtual bool poll();

	void vibrateStrong(float magnitude);
	void vibrateWeak(float magnitude);

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

	bool actionLeft()			{ return button1(); }
	bool actionRight()			{ return button3(); }
	bool actionTop()			{ return button4(); }
	bool actionBottom()			{ return button2(); }

	bool shoulderLeftTop()		{ return button5(); }
	bool shoulderLeftBottom()	{ return button7(); }
	bool shoulderRightTop()		{ return button6(); }
	bool shoulderRightBottom()	{ return button8(); }

	bool select()				{ return button9(); }
	bool start()				{ return button10(); }

	bool button1()				{ return getButtonValue(0); }
	bool button2()				{ return getButtonValue(1); }
	bool button3()				{ return getButtonValue(2); }
	bool button4()				{ return getButtonValue(3); }
	bool button5()				{ return getButtonValue(4); }
	bool button6()				{ return getButtonValue(5); }
	bool button7()				{ return getButtonValue(6); }
	bool button8()				{ return getButtonValue(7); }
	bool button9()				{ return getButtonValue(8); }
	bool button10()				{ return getButtonValue(9); }

	/*** R2Controller Functions ***/

	virtual bool isConnected()			{ return Device::isConnected(); }
	virtual float feetjoyX()			{ return joyLeftX(); }
	virtual float feetJoyY()			{ return joyLeftY(); }
	virtual float domeJoyX()			{ return joyRightX(); }
	virtual float domeJoyY()			{ return joyRightY(); }
	virtual bool deadMan()				{ return shoulderLeftTop(); }
	virtual bool highSpeedEnable()		{ return shoulderLeftBottom(); }
	virtual bool positiveSound()		{ return actionTop(); }
	virtual bool neutralSound()			{ return actionRight(); }
	virtual bool negativeSound()		{ return actionBottom(); }

protected:
	void writeVibrateEffect();

protected:
	bool canVibrate;
	struct ff_effect* vibrateEffect;

};

} // namespace Devices
} // namespace R2D2

#endif // LOGITECHCRP2_H
