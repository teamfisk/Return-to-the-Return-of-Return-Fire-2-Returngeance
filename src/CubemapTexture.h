#ifndef CubemapTexture_h__
#define CubemapTexture_h__

#include <SOIL.h>

class CubemapTexture
{
public:
	CubemapTexture(
	    std::string posXFile,
	    std::string negXFile,
	    std::string posYFile,
	    std::string negYFile,
	    std::string posZFile,
	    std::string negZFile);
	~CubemapTexture();

	void Load();
	void Bind(GLenum textureSlot);

private:
	bool m_Loaded;
	GLuint m_Texture;
	std::string m_TextureFiles[6];
};

#endif // CubemapTexture_h__