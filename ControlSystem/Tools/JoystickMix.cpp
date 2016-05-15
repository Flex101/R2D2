#include "JoystickMix.h"

#include "math.h"

using namespace R2D2;

float JoystickMix::axesToAngle(float x, float y)
{
	float angle = atan(x / y) * (180.0f / M_PI);

	if (x > 0)
	{
		if (y < 0) angle = -angle;
		else angle = 180 - angle;
	}
	else
	{
		if (y < 0) angle = -angle;
		else angle = -180 - angle;
	}

	return angle;
}

float JoystickMix::axesToMagnitude(float x, float y)
{
	return sqrt((x * x) + (y * y)) * squareScale(axesToAngle(x, y));
}

float JoystickMix::squareScale(float angle)
{
	if (fabs(angle) > 45 && fabs(angle) < 135)
	{
		angle = 90 - angle;
	}

	return fabs(cos(angle * (M_PI / 180)));
}
