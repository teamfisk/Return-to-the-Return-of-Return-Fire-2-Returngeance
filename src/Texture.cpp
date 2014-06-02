#include "PrecompiledHeader.h"
#include "Texture.h"

Texture::Texture(std::string path)
{
	Load(path);
}

void Texture::Load(std::string path)
{
	m_Texture = SOIL_load_OGL_texture(path.c_str(), 0, 0, SOIL_FLAG_INVERT_Y);

	if (m_Texture == 0)
	{
		LOG_ERROR("SOIL: Failed to load texture \"%s\"", path.c_str());
		return;
	}

	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_Texture);
}


