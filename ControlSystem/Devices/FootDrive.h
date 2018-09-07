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
	virtual bool reconnect();
	virtual void disconnect();
	virtual bool poll();

	void initialise(SerialDeviceLibrary& library);
	void setReversed(bool reversed);
	void setMaxSpeed(float value);
	void setSpeed(float value);
	void stop()											{ setSpeed(0); }

protected:
	bool commsCheck(unsigned int retryLimit = 3);
	void writeSpeed();

protected:
	std::string ID;
	bool reversed;
	float maxSpeed;
	float speed;
	byte ccCmd[3];
	byte ssCmd[8];
	byte reply[255];
	std::string replyStr;
};

} // namespace Devices
} // namespace R2D2

#endif // FOOTDRIVE_H
