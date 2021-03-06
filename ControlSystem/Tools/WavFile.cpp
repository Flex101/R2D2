#include "WavFile.h"

#include <stdio.h>		// for fopen(), fclose(), fread()
#include <string.h>		// for memset()
#include "Logging.h"

using namespace R2D2;

WavFile::WavFile()
{
	fileLoaded = false;
	filestream = NULL;
}

WavFile::~WavFile()
{

}

bool WavFile::loadFile(std::string filename)
{
	if (fileLoaded) close();

	filestream = fopen(filename.c_str(), "rb");
	fileLoaded = (filestream != NULL);

	if (!fileLoaded) return false;

	return readWavInfo();
}

void WavFile::close()
{
	if (!fileLoaded) return;
	fclose(filestream);
	fileLoaded = false;
}

unsigned int WavFile::streamFrames(short* buffer, unsigned int nFrames)
{
	if (!fileLoaded) return 0;

	size_t size = (info.bitsPerSample / 8) * info.numChannels;
	size_t count = nFrames;// / size;
	size_t read = fread(buffer, size, count, filestream);

	if (read < count)
	{
		fileLoaded = false;
		int err = fclose(filestream);
		if (err != 0) Logging::log(LOG_ERROR, "WAVFILE", "Close failed");
	}

	return (read * size);
}

bool WavFile::readWavInfo()
{
	if (fread(&info.chunkID, sizeof(char), 4 , filestream) < 4) return false;
	if (fread(&info.chunkSize, sizeof(uint32_t), 1, filestream) < 1) return false;
	if (fread(&info.format, sizeof(char), 4, filestream) < 4) return false;
	if (fread(&info.subChunk1ID, sizeof(char), 4, filestream) < 4) return false;
	if (fread(&info.subChunk1Size, sizeof(uint32_t), 1, filestream) < 1) return false;
	if (fread(&info.audioFormat, sizeof(uint16_t), 1, filestream) < 1) return false;
	if (fread(&info.numChannels, sizeof(uint16_t), 1, filestream) < 1) return false;
	if (fread(&info.sampleRate, sizeof(uint32_t), 1, filestream) < 1) return false;
	if (fread(&info.byteRate, sizeof(uint32_t), 1, filestream) < 1) return false;
	if (fread(&info.blockAlign, sizeof(uint16_t), 1, filestream) < 1) return false;
	if (fread(&info.bitsPerSample, sizeof(uint16_t), 1, filestream) < 1) return false;

	char dataStart[4];
	if (fread(&dataStart, sizeof(char), 4 , filestream) < 4) return false;

	Logging::log(LOG_DEBUG, "WAVFILE", "chunkID: %s", std::string((char*)&info.chunkID).substr(0, 4).c_str());
	Logging::log(LOG_DEBUG, "WAVFILE", "chunkSize: %d", info.chunkSize);
	Logging::log(LOG_DEBUG, "WAVFILE", "format: %s", std::string((char*)&info.format).substr(0, 4).c_str());
	Logging::log(LOG_DEBUG, "WAVFILE", "subChunk1ID: %s", std::string((char*)&info.subChunk1ID).substr(0, 4).c_str());
	Logging::log(LOG_DEBUG, "WAVFILE", "subChunk1Size: %d", info.subChunk1Size);
	Logging::log(LOG_DEBUG, "WAVFILE", "audioFormat: %d", info.audioFormat);
	Logging::log(LOG_DEBUG, "WAVFILE", "numChannels: %d", info.numChannels);
	Logging::log(LOG_DEBUG, "WAVFILE", "sampleRate: %d", info.sampleRate);
	Logging::log(LOG_DEBUG, "WAVFILE", "byteRate: %d", info.byteRate);
	Logging::log(LOG_DEBUG, "WAVFILE", "blockAlign: %d", info.blockAlign);
	Logging::log(LOG_DEBUG, "WAVFILE", "bitsPerSample: %d", info.bitsPerSample);
	Logging::log(LOG_DEBUG, "WAVFILE", "dataStart: %s", &dataStart);

	return true;
}

