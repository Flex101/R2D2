#include "SerialDeviceLibrary.h"

#include <fcntl.h>				// for open()
#include <unistd.h>				// for read() and close()
#include <string.h>				// for memset
#include <vector>				// for std::vector
#include "File.h"
#include "Logging.h"

using namespace R2D2;

void SerialDeviceLibrary::initialise()
{
	Logging::log(LOG_INFO, "SERIALLIB", "Searching for serial devices...");

	std::vector<std::string> devices;
	File::listLinks(devices, "/dev/serial/by-path/");

	Logging::log(LOG_DEBUG, "SERIALLIB", "Found %d serial device(s)", devices.size());

	byte ccCmd[] = "CC\r";
	byte idCmd[] = "ID\r";
	byte reply[10];
	std::string replyStr;

	for (unsigned int i = 0; i < devices.size(); ++i)
	{
		device.initialise(devices.at(i), B115200);
		bool gotID = false;

		if (device.connect())
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				if (device.writeData((byte*)&ccCmd, 3))
				{
					memset(&reply, 0, sizeof(reply));
					int bytesRead = device.readLine((byte*)&reply);
					if (bytesRead < 0) continue;

					replyStr = (char*)&reply;
					if (replyStr != "CCOK") continue;

					if (device.writeData((byte*)&idCmd, 3))
					{
						bytesRead = device.readLine((byte*)&reply);
						if (bytesRead < 0) break;

						replyStr = (char*)&reply;
						pathMap[replyStr] = devices.at(i);
						gotID = true;
						Logging::log(LOG_DEBUG, "SERIALLIB", "Identified device: \"%s\"", replyStr.c_str());
						break;
					}
				}
			}

			if (!gotID) Logging::log(LOG_ERROR, "SERIALLIB", "Failed to read Device ID of %s", devices.at(i).c_str());

			device.disconnect();
		}
		else
		{
			Logging::log(LOG_WARN, "SERIALLIB", "Could not connect to %s", devices.at(i).c_str());
		}
	}
}

std::string SerialDeviceLibrary::pathFromID(std::string ID)
{
	PathMap::iterator i = pathMap.find(ID);
	if (i == pathMap.end()) return "";

	return i->second;
}
