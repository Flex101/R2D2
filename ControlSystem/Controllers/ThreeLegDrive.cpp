#include "ThreeLegDrive.h"

#include "Tools/JoystickMix.h"
#include "Tools/Logging.h"

using namespace R2D2;
using namespace R2D2::Controllers;

ThreeLegDrive::ThreeLegDrive()
{
}

ThreeLegDrive::~ThreeLegDrive()
{

}

void ThreeLegDrive::setInput(float x, float y)
{
	if (x == 0 && y == 0)
	{
		leftFootSpeed = 0;
		rightFootSpeed = 0;
		return;
	}

	float angle = JoystickMix::axesToAngle(x, y);
	float magnitude = JoystickMix::axesToMagnitude(x, y);

	float left = magnitude;
	float right = magnitude;

	if (angle > 0)
	{
		if (angle < 90)
		{
			left *= 1;
			right *= (1 - (angle / 45));
		}
		else
		{
			left *= (1 - ((angle - 90) / 45));
			right *= -1;
		}
	}
	else
	{
		if (angle > -90)
		{
			left *= (1 + (angle / 45));
			right *= 1;
		}
		else
		{
			left *= -1;
			right *= (1 + ((angle + 90) / 45));
		}
	}

	leftFootSpeed = left;
	rightFootSpeed = right;

	Logging::log(LOG_DEBUG, "3LEG", "Left: %f Right: %f", left, right);
}
