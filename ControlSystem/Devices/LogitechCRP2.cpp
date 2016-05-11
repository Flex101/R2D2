#include "LogitechCRP2.h"

#include <stdlib.h>			// for malloc() and free()
#include <unistd.h>			// for write()
#include <linux/input.h>	// for ff_effect
#include <string.h>			// for memset
#include "Logging.h"

using namespace R2D2;
using namespace R2D2::Devices;

LogitechCRP2::LogitechCRP2() : Gamepad("Logitech_Logitech_Cordless_RumblePad_2")
{
	vibrateEffect = (ff_effect*)malloc(sizeof(ff_effect));
	canVibrate = true;
}

LogitechCRP2::~LogitechCRP2()
{	
	free(vibrateEffect);
}

void LogitechCRP2::disconnect()
{
	if (connected)
	{
		vibrateStrong(0);
		vibrateWeak(0);
		writeVibrateEffect();
	}

	Gamepad::disconnect();
}

void LogitechCRP2::initialise()
{
	Gamepad::initialise();
	if (output_id < 0) return;

	memset(&outputEvent, 0, sizeof(JoyOutputEvent));
	outputEvent.type = EV_FF;
	outputEvent.value = 1;

	memset(vibrateEffect, 0, sizeof(ff_effect));
	vibrateEffect->type = FF_RUMBLE;
	vibrateEffect->id = -1;
	vibrateEffect->u.rumble.strong_magnitude = 0x0;
	vibrateEffect->u.rumble.weak_magnitude = 0x0;
	vibrateEffect->replay.length = 0;
	vibrateEffect->replay.delay = 0;
}

void LogitechCRP2::poll()
{
	Gamepad::poll();
	if (output_id > 0 && canVibrate) writeVibrateEffect();
}

void LogitechCRP2::vibrateStrong(float magnitude)
{
	if (magnitude > 1) magnitude = 1;
	vibrateEffect->u.rumble.strong_magnitude = (0xFFFF * magnitude);
}

void LogitechCRP2::vibrateWeak(float magnitude)
{
	if (magnitude > 1) magnitude = 1;
	vibrateEffect->u.rumble.weak_magnitude = (0xFFFF * magnitude);
}

void LogitechCRP2::writeVibrateEffect()
{
	if (ioctl(output_id, EVIOCSFF, vibrateEffect) < 0)
	{
		Logging::log(LOG_WARN, "GAMEPAD", "Vibrate config failed");
		canVibrate = false;
		return;
	}
	outputEvent.code = vibrateEffect->id;

	int bytes = write(output_id, (const void*)&outputEvent, sizeof(JoyOutputEvent));
	if (bytes < 0) Logging::log(LOG_ERROR, "Vibrate write failed");
}
