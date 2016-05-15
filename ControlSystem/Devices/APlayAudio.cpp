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
	playing = false;

	playWavScript = File::applicationDirectory() + "/Resources/Scripts/playWav.sh";
}

Audio::~Audio()
{
}

bool Audio::connect()
{
	connected = File::fileExists(playWavScript);

	if (connected) Logging::log(LOG_INFO, "AUDIO", "Connected");
	else Logging::log(LOG_ERROR, "AUDIO", "Failed to connect");

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

	int result = Process::execute("bash " + playWavScript + " \"" + filename + "\"");
	Logging::log(LOG_INFO, "AUDIO", "Playing %s ...", filename.c_str());
	playing = true;

	return (result > 0);
}

