#ifndef AUDIO_H
#define AUDIO_H

#include "Device.h"
#include <string>

namespace R2D2 {
namespace Devices {

class Audio : public Device
{
public:
	explicit Audio(std::string playbackDevice);
	virtual ~Audio();

	virtual bool connect();
	virtual void disconnect();

	virtual void poll();

	bool playWavFile(std::string filename);

	bool isPlaying() { return playing; }

protected:
	std::string playbackDevice;
	bool playing;
};

} // namespace Devices
} // namespace R2D2

#endif // AUDIO_H
