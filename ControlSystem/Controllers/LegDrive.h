#ifndef LEGDRIVE_H
#define LEGDRIVE_H

namespace R2D2 {
namespace Controllers {

class LegDrive
{
public:
	explicit LegDrive();
	virtual ~LegDrive();

	virtual void setInput(float x, float y) = 0;

	float getLeftFootSpeed() const		{ return leftFootSpeed; }
	float getRightFootSpeed() const		{ return rightFootSpeed; }

protected:
	float leftFootSpeed;
	float rightFootSpeed;
};

} // namespace Controllers
} // namespace R2D2

#endif // LEGDRIVE_H
