#include "R2CS.h"

#include "LogitechCRP2.h"
#include "APlayAudio.h"
#include "AudioLibrary.h"
#include "SerialDeviceLibrary.h"
#include "FootDrive.h"
#include "DomeDrive.h"
#include "ThreeLegDrive.h"
#include "TwoLegDrive.h"
#include "File.h"
#include "RealTime.h"
#include "Logging.h"

using namespace R2D2;
using namespace R2D2::Devices;
using namespace R2D2::Controllers;

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

	domeDrive = new DomeDrive();
	threeLegDrive = new ThreeLegDrive();
	twoLegDrive = new TwoLegDrive();

	// Reduce max speed to 25%
	leftFoot->setMaxSpeed(0.25f);
	rightFoot->setMaxSpeed(0.25f);

	// Left foot needs to be reversed due to orientation of the motor
	leftFoot->setReversed(true);
	rightFoot->setReversed(false);

	footDrivesEnabled = false;
	onTwoLegs = false;
	stopRequest = false;
	stopSilent = false;
	goodStartupWav = File::applicationDirectory() + "/Resources/Audio/Positive/I Agree.wav";
	badStartupWav = File::applicationDirectory() + "/Resources/Audio/Negative/Weeoooowww!.wav";
	shutdownWav = File::applicationDirectory() + "/Resources/Audio/Negative/Building Freak Out.wav";
}

R2CS::~R2CS()
{
	delete domeDrive;
	delete threeLegDrive;
	delete twoLegDrive;

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
		Logging::log(LOG_WARN, "CS", "Foot Drive system failure - IMMOBILISED");
		leftFoot->disconnect();
		rightFoot->disconnect();
		audio->playWavFile(badStartupWav);
	}
	else
	{
		audio->playWavFile(goodStartupWav);
		footDrivesEnabled = true;
	}

	Logging::log(LOG_INFO, "CS", "Control loop started...");

	while (!stopRequest)
	{
		if (footDrivesEnabled && (!leftFoot->isConnected() || !rightFoot->isConnected()))
		{
			Logging::log(LOG_WARN, "CS", "Foot Drive system failure - IMMOBILISED");
			leftFoot->disconnect();
			rightFoot->disconnect();
			audio->playWavFile(badStartupWav);
			footDrivesEnabled = false;
		}

		for (unsigned int i = 0; i < devices.size(); i++)
		{
			if(devices.at(i)->isConnected()) devices.at(i)->poll();
		}

		/*** Vibrate Tests ***/
		if (gamepad->isConnected())
		{
			if (gamepad->shoulderLeftTop()) gamepad->vibrateStrong(1);
			else gamepad->vibrateStrong(0);

			if (gamepad->shoulderRightTop()) gamepad->vibrateWeak(1);
			else gamepad->vibrateWeak(0);
		}

		/*** Sound Control ***/
		if (gamepad->isConnected() && audio->isConnected())
		{
			if (!audio->isPlaying())
			{
				if (gamepad->dPadDown()) audio->playWavFile(negativeSounds.random());
				if (gamepad->dPadLeft()) audio->playWavFile(neutralSounds.random());
				if (gamepad->dPadUp()) audio->playWavFile(positiveSounds.random());
			}
		}

		/*** Foot Drive Control ***/
		if (gamepad->isConnected() && footDrivesEnabled)
		{
			LegDrive* drive = onTwoLegs ? static_cast<LegDrive*>(twoLegDrive) : static_cast<LegDrive*>(threeLegDrive);

			drive->setInput(gamepad->joyLeftX(), gamepad->joyLeftY());
			leftFoot->setSpeed(drive->getLeftFootSpeed());
			rightFoot->setSpeed(drive->getRightFootSpeed());
		}

		RealTime::sleepMilli(10);
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
	if (stopRequest)
	{
		Logging::log(LOG_ERROR, "CS", "Kill request");
		delete this;
		return;
	}

	Logging::log(LOG_WARN, "CS", "Requested to stop");
	stopSilent = silent;
	stopRequest = true;
}
