#include "Audio.h"

#include <stdlib.h>			// for free()
#include <pulse/simple.h>
#include <pulse/error.h>
#include "Logging.h"

using namespace R2D2;
using namespace Devices;

Audio::Audio(std::string device)
{
	playbackDevice = device;
	playing = false;
	buffer = NULL;
	pulse = NULL;
	buffer_size = 1024;

	buffer = (short*)malloc(sizeof(short) * buffer_size);
}

Audio::~Audio()
{
	if (pulse != NULL) pa_simple_free(pulse);
	free(buffer);
}

bool Audio::connect()
{
	pa_sample_spec sampleSpec;
	sampleSpec.format = PA_SAMPLE_U8;
	sampleSpec.rate = 44100;
	sampleSpec.channels = 2;

	if (!(pulse = pa_simple_new(NULL, "R2D2", PA_STREAM_PLAYBACK, NULL, "playback", &sampleSpec, NULL, NULL, &error)))
	{
		Logging::log(LOG_ERROR, "Configuration error: %s", pa_strerror(error));
		return false;
	}

	connected = true;
	Logging::log(LOG_INFO, "AUDIO", "Connected");

	return connected;
}

void Audio::disconnect()
{	
	if (!connected) return;
	Logging::log(LOG_INFO, "AUDIO", "Disconnected");
}

void Audio::poll()
{
	pa_usec_t latency;

	if ((latency = pa_simple_get_latency(pulse, &error)) == (pa_usec_t)-1)
	{
		Logging::log(LOG_WARN, "AUDIO", "Poll error: %s", pa_strerror(error));
		return;
	}

	if (wavFile.isFileLoaded())
	{
		int framesRead = wavFile.streamFrames(buffer, buffer_size);

		if (pa_simple_write(pulse, buffer, (size_t)framesRead, &error) < 0)
		{
			Logging::log(LOG_WARN, "AUDIO", "Write error: %s", pa_strerror(error));
		}

		if (framesRead < buffer_size)
		{
			pa_simple_drain(pulse, &error);
			playing = false;
		}
	}
}

bool Audio::playWavFile(std::string filename)
{
	Logging::log(LOG_INFO, "AUDIO", "Playing %s ...", filename.c_str());
	bool success = wavFile.loadFile(filename);
	playing = true;
	return success;
}

