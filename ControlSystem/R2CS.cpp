#include "R2CS.h"

#include "LogitechCRP2.h"
#include "APlayAudio.h"
#include "AudioLibrary.h"
#include "SerialDeviceLibrary.h"
#include "FootDrive.h"
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

	leftFoot = new FootDrive("LFOOT");
	rightFoot = new FootDrive("RFOOT");

	devices.push_back(leftFoot);
	devices.push_back(rightFoot);

	stopRequest = false;
	stopSilent = false;
	goodStartupWav = File::applicationDirectory() + "/Resources/Audio/Positive/I Agree.wav";
	badStartupWav = File::applicationDirectory() + "/Resources/Audio/Negative/Weeoooowww!.wav";
	shutdownWav = File::applicationDirectory() + "/Resources/Audio/Negative/Building Freak Out.wav";
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

	serialDevices.initialise();
	leftFoot->initialise(serialDevices);
	rightFoot->initialise(serialDevices);

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

	positiveSounds.readFromDir(File::applicationDirectory() + "/Resources/Audio/Positive", true);
	neutralSounds.readFromDir(File::applicationDirectory() + "/Resources/Audio/Neutral", true);
	negativeSounds.readFromDir(File::applicationDirectory() + "/Resources/Audio/Negative", true);
}

void R2CS::start()
{
	if (!leftFoot->isConnected() || !rightFoot->isConnected())
	{
		Logging::log(LOG_WARN, "CS", "Drive system failure - IMMOBILISED");
		leftFoot->disconnect();
		rightFoot->disconnect();
		audio->playWavFile(badStartupWav);
	}
	else
	{
		audio->playWavFile(goodStartupWav);
	}

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
				if (gamepad->dPadDown()) audio->playWavFile(negativeSounds.random());
				if (gamepad->dPadLeft()) audio->playWavFile(neutralSounds.random());
				if (gamepad->dPadUp()) audio->playWavFile(positiveSounds.random());
			}
		}

		RealTime::sleepMilli(1);
	}

	Logging::log(LOG_INFO, "CS", "Control loop stopped");

	if (!stopSilent && !audio->isPlaying())
	{
		audio->playWavFile(shutdownWav);

		while (audio->isPlaying())
		{
			audio->poll();
			RealTime::sleepMilli(1);
		}
	}
}

void R2CS::stop(bool silent)
{
	Logging::log(LOG_WARN, "CS", "Requested to stop");
	stopSilent = silent;
	stopRequest = true;
}
