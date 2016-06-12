#ifndef DOME_H
#define DOME_H

#include "FootDrive.h"

namespace R2D2 {
namespace Devices {

class Dome : public FootDrive
{
public:
	explicit Dome();
	virtual ~Dome();
};

} // namespace Devices
} // namespace R2D2

#endif // DOME_H
