#include "Audio.h"

#include "Logging.h"

using namespace R2D2;
using namespace Devices;

Audio::Audio(std::string device)
{
	playbackDevice = device;
	playing = false;
}

Audio::~Audio()
{
}

bool Audio::connect()
{
	unsigned int rate = 44100;

	err = 0;
	if (err >= 0) err = snd_pcm_open(&playback_handle, playbackDevice.c_str(), SND_PCM_STREAM_PLAYBACK, 0);
	if (err >= 0) err = snd_pcm_hw_params_malloc(&hw_params);
	if (err >= 0) err = snd_pcm_hw_params_any(playback_handle, hw_params);
	if (err >= 0) err = snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err >= 0) err = snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_U16_LE);
	if (err >= 0) err = snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &rate, 0);
	if (err >= 0) err = snd_pcm_hw_params_set_rate_resample(playback_handle, hw_params, 1);
	if (err >= 0) err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, 1);
	if (err >= 0) err = snd_pcm_hw_params(playback_handle, hw_params);
	if (err >= 0) snd_pcm_hw_params_free(hw_params);

	if (err >= 0) err = snd_pcm_sw_params_malloc(&sw_params);
	if (err >= 0) err = snd_pcm_sw_params_current(playback_handle, sw_params);
	if (err >= 0) err = snd_pcm_sw_params_set_avail_min(playback_handle, sw_params, 4096);
	if (err >= 0) err = snd_pcm_sw_params_set_start_threshold(playback_handle, sw_params, 0U);
	if (err >= 0) err = snd_pcm_sw_params(playback_handle, sw_params);

	if (err < 0)
	{
		Logging::log(LOG_ERROR, "AUDIO", snd_strerror(err));
		Logging::log(LOG_ERROR, "AUDIO", "Failed to connect");

	}
	else Logging::log(LOG_INFO, "AUDIO", "Connected");

	connected = (err >= 0);

	return connected;
}

void Audio::disconnect()
{
	if (!connected) return;
	snd_pcm_close(playback_handle);
	Logging::log(LOG_INFO, "AUDIO", "Disconnected");
}

void Audio::poll()
{
	int result = (frames_to_deliver = snd_pcm_avail_update(playback_handle));

	if (result < 0)
	{
		Logging::log(LOG_WARN, "AUDIO", "Update failed - %s", snd_strerror(result));
		snd_pcm_recover(playback_handle, result, 0);
		return;
	}

	if (frames_to_deliver > 0)
	{
		playing = wavFile.isFileLoaded();

		if (frames_to_deliver > 4096) frames_to_deliver = 4096;

		if (!deliverFrames(frames_to_deliver))
		{
			Logging::log(LOG_WARN, "AUDIO", "Playback callback failed");
			wavFile.close();
		}
	}
}

bool Audio::loadWavFile(std::string filename)
{
	Logging::log(LOG_INFO, "AUDIO", "Playing %s ...", filename.c_str());
	bool success = wavFile.loadFile(filename);
	if (success) snd_pcm_prepare(playback_handle);
	playing = true;
	return success;
}

bool Audio::deliverFrames(snd_pcm_sframes_t requestedframes)
{
	int writtenFrames = wavFile.streamFrames((short*)&buf, (unsigned int)requestedframes);

	for(; writtenFrames < requestedframes; ++writtenFrames)
	{
		buf[writtenFrames] = 0;
	}

	if ((err = snd_pcm_writei(playback_handle, &buf, writtenFrames)) < 0)
	{
		Logging::log(LOG_WARN, "AUDIO", "Write failed - %s", snd_strerror (err));
	}

	return (err == writtenFrames);
}

