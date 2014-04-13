#ifndef Sound_h__
#define Sound_h__

#include <AL/al.h>
#include <AL/alc.h>

#include "ResourceManager.h"

class Sound : public Resource
{
public:
	Sound(std::string path);

	ALuint LoadFile(std::string path);

	operator ALuint() const { return m_Buffer; }

private:
	ALuint m_Buffer;
};

#endif // Sound_h__
