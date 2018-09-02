#ifndef R2CS_H
#define R2CS_H

#include <vector>
#include "Tools/SerialDeviceLibrary.h"
#include "Tools/AudioLibrary.h"

namespace R2D2 {

namespace Devices { class Device; class LogitechCRP2; class Audio; class FootDrive; class Dome; }
namespace Controllers { class DomeDrive; class ThreeLegDrive; class TwoLegDrive; }

class AudioLibrary;

class R2CS
{
public:
	explicit R2CS();
	virtual ~R2CS();

	void connectToDevices();
	void initialise();
	void start();
	void stop(bool silent = false);

protected:

protected:
	/*** Devices ***/
	std::vector<Devices::Device*> devices;
	SerialDeviceLibrary serialDevices;

	Devices::LogitechCRP2* gamepad;
	Devices::Audio* audio;
	Devices::FootDrive* leftFoot;
	Devices::FootDrive* rightFoot;
	Devices::Dome* dome;

	Controllers::DomeDrive* domeDrive;
	Controllers::ThreeLegDrive* threeLegDrive;
	Controllers::TwoLegDrive* twoLegDrive;

	AudioLibrary positiveSounds;
	AudioLibrary neutralSounds;
	AudioLibrary negativeSounds;

	std::string goodStartupWav;
	std::string badStartupWav;
	std::string shutdownWav;

	bool footDrivesEnabled;
	bool onTwoLegs;
	bool stopRequest;
	bool stopSilent;
};

} // namespace R2D2

#endif // R2CS_H
