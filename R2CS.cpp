#include "R2CS.h"

#include "LogitechCRP2.h"
#include "Audio.h"
#include "AudioLibrary.h"
#include "File.h"
#include "RealTime.h"
#include "Logging.h"

using namespace R2D2;
using namespace R2D2::Devices;

R2CS::R2CS()
{
	gamepad = new LogitechCRP2();
	audio = new Audio("cards.pcm.default");

	devices.push_back(gamepad);
	devices.push_back(audio);

	positiveSounds = new AudioLibrary();
	neutralSounds = new AudioLibrary();
	negativeSounds = new AudioLibrary();

	stopRequest = false;
}

R2CS::~R2CS()
{
	for (unsigned int i = 0; i < devices.size(); i++)
	{
		devices.at(i)->disconnect();
		delete devices.at(i);
	}
	devices.clear();
}

void R2CS::connectToDevices()
{
	Logging::log(LOG_INFO, "CS", "Connecting to devices...");

	bool connectedToAll = true;

	for (unsigned int i = 0; i < devices.size(); i++)
	{
		connectedToAll &= devices.at(i)->connect();
	}

	if (connectedToAll) Logging::log(LOG_INFO, "CS", "Connected to all devices");
	else Logging::log(LOG_WARN, "CS", "Unable to connect to all devices");
}

void R2CS::initialise()
{
	Logging::log(LOG_INFO, "CS", "Initialising...");

	positiveSounds->readFromDir(File::workingDirectory() + "/Resources/Audio/Positive", true);
	neutralSounds->readFromDir(File::workingDirectory() + "/Resources/Audio/Neutral", true);
	negativeSounds->readFromDir(File::workingDirectory() + "/Resources/Audio/Negative", true);
}

void R2CS::start()
{
	Logging::log(LOG_INFO, "CS", "Control loop started...");

	while (!stopRequest)
	{
		for (unsigned int i = 0; i < devices.size(); i++)
		{
			if(devices.at(i)->isConnected()) devices.at(i)->poll();
		}

		if (gamepad->isConnected())
		{
			if (gamepad->shoulderLeftTop()) gamepad->vibrateStrong(1);
			else gamepad->vibrateStrong(0);


			if (gamepad->shoulderRightTop()) gamepad->vibrateWeak(1);
			else gamepad->vibrateWeak(0);

			if (audio->isConnected() && !audio->isPlaying())
			{
				if (gamepad->dPadDown()) audio->loadWavFile(negativeSounds->random());
				if (gamepad->dPadLeft()) audio->loadWavFile(neutralSounds->random());
				if (gamepad->dPadUp()) audio->loadWavFile(positiveSounds->random());
			}
		}

		RealTime::sleepMilli(1);
	}

	Logging::log(LOG_INFO, "CS", "Control loop stopped");
}

void R2CS::stop()
{
	Logging::log(LOG_WARN, "CS", "Requested to stop");
	stopRequest = true;
}
