#ifndef DEVICE_H
#define DEVICE_H

namespace R2D2 {
namespace Devices {

class Device
{
public:
	explicit Device();
	virtual ~Device() {}

	virtual bool connect() = 0;
	virtual bool reconnect();
	virtual void disconnect() = 0;

	bool isConnected() { return connected; }

	virtual bool poll() = 0;

protected:
	bool connected;
};

} // namespace Devices
} // namespace R2D2

#endif // DEVICE_H
