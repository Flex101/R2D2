#include "APlayAudio.h"

#include "File.h"
#include "Process.h"
#include "Logging.h"

using namespace R2D2;
using namespace Devices;

Audio::Audio(std::string _playbackDevice)
{
	playbackDevice = _playbackDevice;
	connected = false;
}

Audio::~Audio()
{
}

bool Audio::connect()
{
	connected = true;

	if (!connected) Logging::log(LOG_ERROR, "AUDIO", "Failed to connect");

	return connected;
}

void Audio::disconnect()
{
	if (!connected) return;
	Logging::log(LOG_INFO, "AUDIO", "Disconnected");
}

void Audio::poll()
{
	if (!connected) return;
	playing = Process::isRunning("aplay");
}

bool Audio::playWavFile(std::string filename)
{
	if (!connected) return false;
	if (isPlaying()) return false;

	int result = Process::execute("./Resources/Scripts/playWav.sh \"" + filename + "\"");
	Logging::log(LOG_INFO, "AUDIO", "Playing %s ...", filename.c_str());

	return (result > 0);
}

