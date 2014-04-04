#ifndef Components_SoundEmitter_h__
#define Components_SoundEmitter_h__

#include <string>

#include "Component.h"

namespace Components
{

struct SoundEmitter : Component
{
	float Gain;
	float MaxDistance;
	float ReferenceDistance;
	float Pitch;
	bool Loop;
	std::string Path;
};

}
#endif // !Components_SoundEmitter_h__