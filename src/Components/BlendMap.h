#ifndef Components_BlendMap_h__
#define Components_BlendMap_h__

#include "Component.h"
#include "Entity.h"

namespace Components
{
	struct BlendMap : Component
	{
		BlendMap() 
			: TextureRed("Textures/Ground/Asphalt")
			, TextureGreen("Textures/Ground/Grass")
			, TextureBlue("Textures/Ground/Sand")
			, TextureRepeats(100.f) { }

		std::string TextureRed, TextureGreen, TextureBlue;
		float TextureRepeats;

		virtual BlendMap* Clone() const override { return new BlendMap(*this); }
	};

}
#endif // !Components_BlendMap_h__