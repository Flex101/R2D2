#ifndef DOMEDRIVE_H
#define DOMEDRIVE_H

namespace R2D2 {
namespace Controllers {

class DomeDrive
{
public:
	explicit DomeDrive();
	virtual ~DomeDrive();

	void setInput(float x);
	void getOutput(float& domeDrive);
};

} // namespace Controllers
} // namespace R2D2

#endif // DOMEDRIVE_H
