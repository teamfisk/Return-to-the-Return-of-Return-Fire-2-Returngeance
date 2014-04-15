#include "PrecompiledHeader.h"
#include "Skybox.h"

Skybox::Skybox(std::string skyboxPath, std::string extension /* = "png" */)
{
	m_Cubemap = std::shared_ptr<CubemapTexture>(new CubemapTexture(
			skyboxPath + "/right." + extension,
	                skyboxPath + "/left." + extension,
	                skyboxPath + "/top." + extension,
	                skyboxPath + "/bottom." + extension,
	                skyboxPath + "/front." + extension,
	                skyboxPath + "/back." + extension));
	m_Cubemap->Load();
	Initialize();
}

void Skybox::Initialize()
{
	float cubeVertices[] =
	{
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
	};
	//std::copy(cubeVertices, cubeVertices + (3*8 - 1), m_CubeVertices);

	unsigned int cubeIndices[] =
	{
		// Back
		0, 2, 3,
		0, 1, 2,

		// Right
		1, 6, 2,
		1, 5, 6,

		// Front
		5, 7, 6,
		5, 4, 7,

		// Left
		4, 3, 7,
		4, 0, 3,

		// Top
		3, 6, 7,
		3, 2, 6,

		// Bottom
		4, 1, 0,
		4, 5, 1,
	};
	//std::copy(cubeIndices, cubeIndices + (3*12 - 1), m_CubeIndices);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(float), cubeVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 12 * sizeof(int), cubeIndices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	GLERROR("Skybox init");
}


Skybox::~Skybox()
{

}

void Skybox::Draw()
{
	m_Cubemap->Bind(GL_TEXTURE0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDepthMask(GL_FALSE);
	glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, 0);
	glDepthMask(GL_TRUE);
}
