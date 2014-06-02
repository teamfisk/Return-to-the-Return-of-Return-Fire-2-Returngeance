#include "PrecompiledHeader.h"
#include "Sound.h"


Sound::Sound(std::string path, FMOD_SYSTEM* system, Components::SoundEmitter::SoundType type)
{
	if(type == Components::SoundEmitter::SoundType::SOUND_2D)
	{
		FMOD_System_CreateSound(system, path.c_str(), FMOD_2D, NULL, &m_Sound);
	}
	if(type == Components::SoundEmitter::SoundType::SOUND_3D)
	{
		FMOD_System_CreateSound(system, path.c_str(), FMOD_3D, NULL, &m_Sound);
	}
}

Sound::~Sound()
{
	 FMOD_Sound_Release(m_Sound);
}