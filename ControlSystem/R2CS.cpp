#include "R2CS.h"

#include "Controllers/DomeDrive.h"
#include "Controllers/ThreeLegDrive.h"
#include "Controllers/TwoLegDrive.h"
#include "Devices/PS3Controller.h"
#include "Devices/APlayAudio.h"
#include "Devices/FootDrive.h"
#include "Devices/Dome.h"
#include "Tools/AudioLibrary.h"
#include "Tools/SerialDeviceLibrary.h"
#include "Tools/File.h"
#include "Tools/RealTime.h"
#include "Tools/Logging.h"
#include "Tools/DeviceInfo.h"

using namespace R2D2;
using namespace R2D2::Devices;
using namespace R2D2::Controllers;

R2CS::R2CS()
{
	gamepad = new PS3Controller("00:21:4f:12:b5:13");
	audio = new Audio("cards.pcm.default");

	devices.push_back(gamepad);
	devices.push_back(audio);

	leftFoot = new FootDrive("LFOOT");
	rightFoot = new FootDrive("RFOOT");
	dome = new Dome();

	devices.push_back(leftFoot);
	devices.push_back(rightFoot);
	devices.push_back(dome);

	domeDrive = new DomeDrive();
	threeLegDrive = new ThreeLegDrive();
	twoLegDrive = new TwoLegDrive();

	// Turn off drives at start
	leftFoot->setMaxSpeed(0.0f);
	rightFoot->setMaxSpeed(0.0f);
	dome->setMaxSpeed(0.0f);
	
	// Left foot needs to be reversed due to orientation of the motor
	leftFoot->setReversed(true);
	rightFoot->setReversed(false);
	dome->setReversed(false);

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
	dome->initialise(serialDevices);

	bool connectedToAll = true;

	for (unsigned int i = 0; i < devices.size(); i++)
	{
		connectedToAll &= devices.at(i)->connect();
	}

	footDrivesEnabled = leftFoot->isConnected() && rightFoot->isConnected();

	if (connectedToAll)
	{
		audio->playWavFile(goodStartupWav);
		Logging::log(LOG_INFO, "CS", "Connected to all devices");
	}
	else
	{
		audio->playWavFile(badStartupWav);
		Logging::log(LOG_WARN, "CS", "Unable to connect to all devices");
	}
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
	Logging::log(LOG_INFO, "CS", "Control loop started...");

	while (!stopRequest)
	{
		if (!leftFoot->isConnected()) leftFoot->reconnect();
		if (!rightFoot->isConnected()) rightFoot->reconnect();
		if (!dome->isConnected()) dome->reconnect();

		if (footDrivesEnabled && (!leftFoot->isConnected() || !rightFoot->isConnected()))
		{
			footDrivesEnabled = false;
			Logging::log(LOG_WARN, "CS", "Foot Drive system failure - IMMOBILISED");
		}
		if (!footDrivesEnabled && (leftFoot->isConnected() && rightFoot->isConnected()))
		{
			footDrivesEnabled = true;
			Logging::log(LOG_INFO, "CS", "Foot Drive system restored - DRIVES ENABLED");
		}

		for (unsigned int i = 0; i < devices.size(); i++)
		{
			if (devices.at(i)->isConnected()) devices.at(i)->poll();
		}

		/*** Sound Control ***/
		if (gamepad->isConnected() && audio->isConnected())
		{
			if (!audio->isPlaying())
			{
				if (gamepad->positiveSound()) audio->playWavFile(positiveSounds.random());
				if (gamepad->neutralSound()) audio->playWavFile(neutralSounds.random());
				if (gamepad->negativeSound()) audio->playWavFile(negativeSounds.random());
			}
		}

		/*** Foot Drive Control ***/
		if (footDrivesEnabled)
		{
			if (gamepad->isConnected())
			{
				LegDrive* drive = onTwoLegs ? static_cast<LegDrive*>(twoLegDrive) : static_cast<LegDrive*>(threeLegDrive);

				drive->setInput(gamepad->feetjoyX(), gamepad->feetJoyY());

				// Dead-man switch
				if (gamepad->deadMan())
				{
					// High speed select
					if (gamepad->highSpeedEnable())
					{
						// High speed - 40%
						leftFoot->setMaxSpeed(0.4f);
						rightFoot->setMaxSpeed(0.4f);
					}
					else
					{
						// Walking pace - 25%
						leftFoot->setMaxSpeed(0.25f);
						rightFoot->setMaxSpeed(0.25f);
					}
				}
				else
				{
					leftFoot->setMaxSpeed(0.0f);
					rightFoot->setMaxSpeed(0.0f);
				}


				leftFoot->setSpeed(drive->getLeftFootSpeed());
				rightFoot->setSpeed(drive->getRightFootSpeed());
			}
			else
			{
				leftFoot->stop();
				rightFoot->stop();
			}
		}

		/*** Dome Drive Control ***/
		if (dome->isConnected())
		{
			if (gamepad->isConnected())
			{
				domeDrive->setInput(gamepad->domeJoyX());

				// Dead-man switch
				if (gamepad->deadMan())
				{
					// Full Speed - 95%
					dome->setMaxSpeed(0.95f);
				}
				else
				{
					dome->setMaxSpeed(0.0f);
				}

				dome->setSpeed(domeDrive->getDomeSpeed());
			}
			else
			{
				dome->stop();
			}
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
