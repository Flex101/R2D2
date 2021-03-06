#include "APlayAudio.h"

#include "Tools/File.h"
#include "Tools/Process.h"
#include "Tools/Logging.h"

using namespace R2D2;
using namespace Devices;

Audio::Audio(std::string _playbackDevice)
{
	playbackDevice = _playbackDevice;
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

bool Audio::poll()
{
	if (!connected) return false;
	playing = Process::isRunning("aplay");
	return true;
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

