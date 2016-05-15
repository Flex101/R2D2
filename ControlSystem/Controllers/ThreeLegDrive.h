#ifndef THREELEGDRIVE_H
#define THREELEGDRIVE_H

#include "LegDrive.h"

namespace R2D2 {
namespace Controllers {

class ThreeLegDrive : public LegDrive
{
public:
	explicit ThreeLegDrive();
	virtual ~ThreeLegDrive();

	void setInput(float x, float y);
};

} // namespace Controllers
} // namespace R2D2

#endif // THREELEGDRIVE_H
