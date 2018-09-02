#ifndef SERIALDEVICELIBRARY_H
#define SERIALDEVICELIBRARY_H

#include <map>
#include <string>
#include "Devices/SerialDevice.h"

namespace R2D2 {

class SerialDeviceLibrary
{
public:
	explicit SerialDeviceLibrary() {}
	virtual ~SerialDeviceLibrary() {}

	void initialise();

	std::string pathFromID(std::string ID);

protected:
	typedef std::map<std::string, std::string> PathMap;
	PathMap pathMap;
	Devices::SerialDevice device;
};

} // namespace R2D2

#endif // SERIALDEVICELIBRARY_H
