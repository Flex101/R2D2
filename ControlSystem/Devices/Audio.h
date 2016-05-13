#ifndef AUDIO_H
#define AUDIO_H

#include "Device.h"
#include "WavFile.h"
#include <alsa/asoundlib.h>
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

	bool loadWavFile(std::string filename);

	bool isPlaying() { return playing; }

protected:
	bool deliverFrames(snd_pcm_sframes_t nframes);

protected:
	std::string playbackDevice;
	snd_pcm_t* playback_handle;
	snd_pcm_hw_params_t* hw_params;
	snd_pcm_sw_params_t* sw_params;
	snd_pcm_sframes_t frames_to_deliver;

	bool playing;

	WavFile wavFile;

	int err;
	int buffer_length;
	short* buffer;
};

} // namespace Devices
} // namespace R2D2

#endif // AUDIO_H
