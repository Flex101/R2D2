#ifndef JOYSTICKMIX_H
#define JOYSTICKMIX_H

namespace R2D2 {

class JoystickMix
{
public:
	static float axesToAngle(float x, float y);
	static float axesToMagnitude(float x, float y);

private:
	static float squareScale(float angle);
};

} // namespace R2D2

#endif // JOYSTICKMIX_H
