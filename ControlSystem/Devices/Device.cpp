#include "Device.h"

using namespace R2D2;
using namespace R2D2::Devices;

Device::Device()
{
	connected = false;
}

bool Device::reconnect()
{
	return connect();
}

