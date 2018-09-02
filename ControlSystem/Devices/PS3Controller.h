#ifndef PS3CONTROLLER_H
#define PS3CONTROLLER_H

#include "Devices/R2Controller.h"
#include "Devices/Gamepad.h"

#endif // PS3CONTROLLER_H

namespace R2D2 {
namespace Devices {

class PS3Controller : public R2Controller
{
public:
	explicit PS3Controller() {}
	virtual ~PS3Controller() {}

};

} // namespace Devices
} // namespace R2D2
