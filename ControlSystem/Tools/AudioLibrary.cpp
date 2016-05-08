#include "AudioLibrary.h"

#include <stdlib.h>		// for rand()
#include "File.h"
#include "Logging.h"

using namespace R2D2;

AudioLibrary::AudioLibrary()
{

}

AudioLibrary::~AudioLibrary()
{

}

void AudioLibrary::readFromDir(std::string dir, bool recursive)
{
	Logging::log(LOG_INFO, "AUDIOLIB" ,"Populating audio from: %s...", dir.c_str());
	File::listFiles(fileList, dir, recursive, "wav");
	Logging::log(LOG_INFO, "AUDIOLIB", "%d files found", fileList.size());
}

std::string AudioLibrary::at(unsigned int index) const
{
	if (index > fileList.size()) return "";
	return fileList.at(index);
}

std::string AudioLibrary::random()
{
	if (fileList.empty()) return "";
	return fileList.at(rand() % fileList.size());
}
