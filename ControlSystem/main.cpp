#include <iostream>
#include "R2CS.h"
#include "Logging.h"
#include <csignal>		// for SIGINT

using namespace std;
using namespace R2D2;

static R2CS* controlSystem;

static void signalHandler(int signum)
{
	Logging::clearLine();	// Clear any echo of Ctrl+C

	switch (signum)
	{
		case SIGINT:
			controlSystem->stop(true);
			break;

		case SIGTSTP:
			controlSystem->stop();
			break;
	}
}

int main()
{
	Logging::log("--- R2D2 Control System: STARTED ---");

	controlSystem = new R2CS();

	// register signal SIGINT and signal handler
	signal(SIGINT, signalHandler);

	controlSystem->connectToDevices();
	controlSystem->initialise();
	controlSystem->start();

	delete controlSystem;

	Logging::log("--- R2D2 Control System: ENDED ---");
	Logging::newLine();

    return 0;
}

