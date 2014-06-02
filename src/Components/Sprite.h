#ifndef Components_Sprite_h__
#define Components_Sprite_h__

#include <string>

#include "Component.h"

namespace Components
{

struct Sprite : Component
{
	Sprite() : Color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) { }

	std::string SpriteFile;
	glm::vec4 Color;
	
	virtual Sprite* Clone() const override { return new Sprite(*this); }
};

}
#endif // !Components_Sprite_h__