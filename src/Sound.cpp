#include "PrecompiledHeader.h"
#include "Sound.h"


Sound::Sound(std::string path, FMOD_SYSTEM* system)
{

	std::string str = "";
	for(int i = 7; i < 10; i++)
	{
		str += path[i];
	}

	if(str == "BGM")
	{
		FMOD_System_CreateSound(system, path.c_str(), FMOD_2D, NULL, &m_Sound);
	}
	if (str == "SFX")
	{
		FMOD_System_CreateSound(system, path.c_str(), FMOD_3D, NULL, &m_Sound);
	}
	
}

Sound::~Sound()
{
	 FMOD_Sound_Release(m_Sound);
}