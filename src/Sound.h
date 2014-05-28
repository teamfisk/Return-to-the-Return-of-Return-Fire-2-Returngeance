#ifndef Sound_h__
#define Sound_h__

#include "ResourceManager.h"
#include <fmod.h>
#include <fmod_errors.h>

class Sound : public Resource
{
public:
	Sound(std::string path, FMOD_SYSTEM* system);
	~Sound();

	operator FMOD_SOUND*() const { return m_Sound; }

private:
	FMOD_SOUND* m_Sound;
};

#endif // Sound_h__
