#ifndef AUDIOLIBRARY_H
#define AUDIOLIBRARY_H

#include <vector>
#include <string>

namespace R2D2 {

class AudioLibrary
{
public:
	explicit AudioLibrary() {}
	virtual ~AudioLibrary() {}

	void readFromDir(std::string dir, bool recursive = false);

	unsigned int count() const { return fileList.size(); }
	std::string at(unsigned int index) const;

	std::string random();

protected:
	std::vector<std::string> fileList;
};

} // namespace R2D2

#endif // AUDIOLIBRARY_H
