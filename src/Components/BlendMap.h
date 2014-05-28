#ifndef Components_BlendMap_h__
#define Components_BlendMap_h__

#include "Component.h"
#include "Entity.h"

namespace Components
{
	struct BlendMap : Component
	{
		BlendMap() 
			: TextureRed("Textures/ErrorTextureRed.png")
			, TextureGreen("Textures/ErrorTextureGreen.png")
			, TextureBlue("Textures/ErrorTextureBlue.png")
			, TextureRepeats(100.f) { }

		std::string TextureRed;
		std::string	TextureGreen;
		std::string	TextureBlue;
		float TextureRepeats;

		virtual BlendMap* Clone() const override { return new BlendMap(*this); }
	};

}
#endif // !Components_BlendMap_h__