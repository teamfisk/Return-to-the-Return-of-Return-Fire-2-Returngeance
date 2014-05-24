#ifndef Components_SoundEmitter_h__
#define Components_SoundEmitter_h__

#include <string>

#include "Component.h"

namespace Components
{

struct SoundEmitter : Component
{
	SoundEmitter() : Gain(1.f), MaxDistance(1.f), MinDistance(1.f), Pitch(1.f), Loop(false) {}
	float Gain;
	float MaxDistance;
	float MinDistance;
	float Pitch;
	bool Loop;
	std::string Path;

	virtual SoundEmitter* Clone() const override { return new SoundEmitter(*this); }
};

}
#endif // !Components_SoundEmitter_h__