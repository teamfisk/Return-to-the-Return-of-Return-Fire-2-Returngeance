#ifndef Skybox_h__
#define Skybox_h__

#include <algorithm>

#include "CubemapTexture.h"

class Skybox
{
public:
	Skybox(std::string skyboxPath, std::string extension = "png");
	Skybox(std::shared_ptr<CubemapTexture> cubemap) 
		: m_Cubemap(cubemap) { Initialize(); }
	~Skybox();

	void Draw();

private:
	std::shared_ptr<CubemapTexture> m_Cubemap;

	GLuint ibo;
	GLuint vao;

	float m_CubeVertices[3 * 8];
	int m_CubeIndices[1];

	void Initialize();
};

#endif // Skybox_h__