#ifndef Components_SoundEmitter_h__
#define Components_SoundEmitter_h__

#include <string>

#include "Component.h"

namespace Components
{

struct SoundEmitter : Component
{	float Gain = 1.f;
	float MaxDistance = 1.f;
	float ReferenceDistance = 1.f;
	float Pitch = 1.f;
	bool Loop = false;
	std::string Path;
};

}
#endif // !Components_SoundEmitter_h__