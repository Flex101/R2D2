#ifndef R2CS_H
#define R2CS_H

#include <vector>

namespace R2D2 {

namespace Devices { class Device; class LogitechCRP2; class Audio; }

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
	Devices::LogitechCRP2* gamepad;
	Devices::Audio* audio;

	AudioLibrary* positiveSounds;
	AudioLibrary* neutralSounds;
	AudioLibrary* negativeSounds;

	bool stopRequest;
};

} // namespace R2D2

#endif // R2CS_H
