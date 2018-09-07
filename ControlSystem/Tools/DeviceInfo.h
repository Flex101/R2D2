#ifndef DEVICEINFOREADER_H
#define DEVICEINFOREADER_H

#include <string>
#include <map>
#include <vector>

namespace R2D2 {
namespace Tools {

class DeviceInfoReader
{
	typedef std::pair<std::string, std::string> DeviceProperty;
	typedef std::map<std::string, std::string> DeviceInfo;
	typedef std::vector<DeviceInfo> DeviceInfoList;

public:
	explicit DeviceInfoReader() {}
	virtual ~DeviceInfoReader() {}

	std::string joystickFromMac(std::string mac);

protected:
	bool populateList();
	DeviceInfo* getInfo(std::string uniq);

protected:
	DeviceInfoList deviceInfoList;
};

} // namespace Tools
} // namespace R2D2

#endif // DEVICEINFOREADER_H
