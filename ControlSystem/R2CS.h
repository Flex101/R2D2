#ifndef R2CS_H
#define R2CS_H

#include <vector>
#include "SerialDeviceLibrary.h"
#include "AudioLibrary.h"

namespace R2D2 {

namespace Devices { class Device; class LogitechCRP2; class Audio; class FootDrive; }

class AudioLibrary;

class R2CS
{
public:
	explicit R2CS();
	virtual ~R2CS();

	void connectToDevices();
	void initialise();
	void start();
	void stop();

protected:

protected:
	/*** Devices ***/
	std::vector<Devices::Device*> devices;
	SerialDeviceLibrary serialDevices;

	Devices::LogitechCRP2* gamepad;
	Devices::Audio* audio;
	Devices::FootDrive* leftFoot;
	Devices::FootDrive* rightFoot;

	AudioLibrary positiveSounds;
	AudioLibrary neutralSounds;
	AudioLibrary negativeSounds;

	std::string goodStartupWav;
	std::string badStartupWav;
	std::string shutdownWav;

	bool stopRequest;
};

} // namespace R2D2

#endif // R2CS_H
