#include "Tools/DeviceInfo.h"

#include "StringEx.h"
#include "Logging.h"
#include <fcntl.h>		// for open()
#include <unistd.h>		// for read()
#include <sstream>		// for std::istringstream
#include <iterator>		// for std::istream_iterator

using namespace R2D2;
using namespace R2D2::Tools;

std::string DeviceInfoReader::joystickFromMac(std::string mac)
{
	if (!populateList()) return "";

	for (DeviceInfoList::iterator it = deviceInfoList.begin(); it != deviceInfoList.end(); ++it)
	{
		DeviceInfo::iterator jt = (*it).find("Uniq");
		if (jt == (*it).end()) continue;
		if (jt->second != mac) continue;

		jt = (*it).find("Handlers");
		if (jt == (*it).end()) continue;

		std::istringstream iss(jt->second);
		std::vector<std::string> handlers{std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{}};

		for (std::vector<std::string>::iterator kt = handlers.begin(); kt != handlers.end(); ++kt)
		{
			if (StringEx::startsWith((*kt), "js")) return "/dev/input/" + (*kt);
		}
	}

	return "";
}

bool DeviceInfoReader::populateList()
{
	deviceInfoList.clear();
	char buffer[10240];

	int fd = open("/proc/bus/input/devices", O_RDONLY);
	if (fd < 0) return false;

	int bytesRead = 0;
	int totalBytesRead = 0;
	do
	{
		bytesRead = read(fd, &buffer[bytesRead], sizeof(buffer) - bytesRead);
		totalBytesRead += bytesRead;
	}
	while (bytesRead > 0);

	if (bytesRead < 0) return false;

	Logging::log(LOG_DEBUG, "DEVINFO", "%d / %d", totalBytesRead, sizeof(buffer));

	DeviceInfo info;
	std::string property;
	std::string value;
	int propertyBegin = -1;
	int valueBegin = -1;
	bool quoted = false;

	for (int i = 0; i < totalBytesRead; ++i)
	{
		if (((buffer[i] == ' ') && (!quoted)) || (buffer[i] == '\n'))
		{
			if (propertyBegin > 0)
			{
				if (valueBegin > 0)
				{
					value = std::string(&buffer[valueBegin], &buffer[i]);
					info[property] = value;
					valueBegin = -1;
					Logging::log(LOG_DEBUG, "DEVINFO", "%s = %s", property.c_str(), value.c_str());
				}
				else
				{
					value = std::string(&buffer[propertyBegin], &buffer[i]);
					info[property] += " " + value;
					Logging::log(LOG_DEBUG, "DEVINFO", "%s = %s", property.c_str(), (info[property]).c_str());
				}
			}
			else if (buffer[i-1] == '\n')
			{
				deviceInfoList.push_back(info);
				Logging::log(LOG_DEBUG, "DEVINFO", "---");
			}

			if (buffer[i] == ' ') propertyBegin = (i + 1);
			else propertyBegin = -1;
		}
		else if (buffer[i] == '=')
		{
			property = std::string(&buffer[propertyBegin], &buffer[i]);
			value.clear();
			valueBegin = (i + 1);
		}
		else if (buffer[i] == '\"')
		{
			quoted = !quoted;
		}
	}

	return true;
}
