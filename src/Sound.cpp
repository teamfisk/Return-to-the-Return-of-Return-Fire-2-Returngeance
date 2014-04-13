#include "PrecompiledHeader.h"
#include "Sound.h"

Sound::Sound(std::string path)
{
	m_Buffer = 0;
	m_Buffer = LoadFile(path);
}

ALuint Sound::LoadFile(std::string path)
{
	char type[4];
	unsigned long size, chunkSize;
	short formatType, channels;
	unsigned long sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;
	unsigned long dataSize;

	FILE* fp = NULL;
	fp = fopen(path.c_str(), "rb");

	if (fp == NULL)
	{
		LOG_ERROR("Failed to load sound file \"%s\"", path.c_str());
		return 0;
	}

	//CHECK FOR VALID WAVE-FILE
	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F')
	{
		LOG_ERROR("ERROR: No RIFF in WAVE-file");
		return 0;
	}

	fread(&size, sizeof(unsigned long), 1, fp);
	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E')
	{
		LOG_ERROR("ERROR: Not WAVE-file");
		return 0;
	}

	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ')
	{
		LOG_ERROR("ERROR: No fmt in WAVE-file");
		return 0;
	}

	//READ THE DATA FROM WAVE-FILE
	fread(&chunkSize, sizeof(unsigned long), 1, fp);
	fread(&formatType, sizeof(short), 1, fp);
	fread(&channels, sizeof(short), 1, fp);
	fread(&sampleRate, sizeof(unsigned long), 1, fp);
	fread(&avgBytesPerSec, sizeof(unsigned long), 1, fp);
	fread(&bytesPerSample, sizeof(short), 1, fp);
	fread(&bitsPerSample, sizeof(short), 1, fp);

	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a')
	{
		LOG_ERROR("ERROR: WAVE-file Missing data");
		return 0;
	}

	fread(&dataSize, sizeof(unsigned long), 1, fp);

	unsigned char* buf = new unsigned char[dataSize];
	fread(buf, sizeof(unsigned char), dataSize, fp);
	fclose(fp);

	// Create buffer
	ALuint format = 0;
	if (bitsPerSample == 8)
	{
		if (channels == 1)
			format = AL_FORMAT_MONO8;
		else if (channels == 2)
			format = AL_FORMAT_STEREO8;
	}
	if (bitsPerSample == 16)
	{
		if (channels == 1)
			format = AL_FORMAT_MONO16;
		else if (channels == 2)
			format = AL_FORMAT_STEREO16;
	}

	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, buf, dataSize, sampleRate);
	delete[] buf;

	return buffer;
}
