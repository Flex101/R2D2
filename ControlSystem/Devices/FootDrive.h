#ifndef FOOTDRIVE_H
#define FOOTDRIVE_H

#include "SerialDevice.h"

namespace R2D2 { class SerialDeviceLibrary;
namespace Devices {

class FootDrive : public SerialDevice
{
public:
	explicit FootDrive(std::string ID);
	virtual ~FootDrive();

	virtual bool connect();
	virtual void disconnect();
	virtual void poll();


	void initialise(SerialDeviceLibrary& library);
	void setReversed(bool reversed);
	void setSpeed(float value);

protected:
	void writeSpeed();

protected:
	std::string ID;
	bool reversed;
	float speed;
	byte ssCmd[8];
	byte ssReply[255];
	std::string replyStr;
};

} // namespace Devices
} // namespace R2D2

#endif // FOOTDRIVE_H
