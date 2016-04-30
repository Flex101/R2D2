#ifndef WAVFILE_H
#define WAVFILE_H

#include "CommonDefs.h"
#include <string>

namespace R2D2 {

class WavFile
{

struct WavInfo
{
	char chunkID[4];
	uint32_t chunkSize;
	char format[4];
	char subChunk1ID[4];
	uint32_t subChunk1Size;
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
};

public:
	explicit WavFile();
	virtual ~WavFile();

	bool loadFile(std::string filename);
	void close();

	unsigned int streamFrames(short* buffer, unsigned int nFrames);

	bool isFileLoaded() const { return fileLoaded; }

protected:
	void readWavInfo();

protected:
	bool fileLoaded;
	FILE* filestream;
	WavInfo info;
};

} // namespace R2D2

#endif // WAVFILE_H
