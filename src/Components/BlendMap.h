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
			, TextureRedNormal("Textures/NeutralNormalMap.png")
			, TextureRedSpecular("Textures/NeutralSpecularMap.png")
			, TextureGreen("Textures/ErrorTextureGreen.png")
			, TextureGreenNormal("Textures/NeutralNormalMap.png")
			, TextureGreenSpecular("Textures/NeutralSpecularMap.png")
			, TextureBlue("Textures/ErrorTextureBlue.png")
			, TextureBlueNormal("Textures/NeutralNormalMap.png")
			, TextureBlueSpecular("Textures/NeutralSpecularMap.png")
			, TextureRepeats(100.f) { }

		std::string TextureRed;
		std::string TextureRedNormal;
		std::string TextureRedSpecular;
		std::string	TextureGreen;
		std::string	TextureGreenNormal;
		std::string	TextureGreenSpecular;
		std::string	TextureBlue;
		std::string	TextureBlueNormal;
		std::string	TextureBlueSpecular;
		float TextureRepeats;

		virtual BlendMap* Clone() const override { return new BlendMap(*this); }
	};

}
#endif // !Components_BlendMap_h__