#ifndef TWOLEGDRIVE_H
#define TWOLEGDRIVE_H

#include "LegDrive.h"

namespace R2D2 {
namespace Controllers {

class TwoLegDrive : public LegDrive
{
public:
	explicit TwoLegDrive();
	virtual ~TwoLegDrive();

	void setInput(float x, float y);
};

} // namespace Controllers
} // namespace R2D2

#endif // TWOLEGDRIVE_H
