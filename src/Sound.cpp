#include "PrecompiledHeader.h"
#include "Sound.h"


Sound::Sound(std::string path, FMOD_SYSTEM* system)
{
	FMOD_System_CreateSound(system, path.c_str(), FMOD_3D, NULL, &m_Sound);
}

Sound::~Sound()
{
	 FMOD_Sound_Release(m_Sound);
}