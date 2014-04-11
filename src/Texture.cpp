#include "PrecompiledHeader.h"
#include "Texture.h"

Texture::Texture(std::string path)
{	Load(path);
}

void Texture::Load(std::string path)
{	auto cachedTexture = m_TextureCache.find(path);
	if (cachedTexture == m_TextureCache.end())
	{	m_TextureCache[path] = SOIL_load_OGL_texture(path.c_str(), 0, 0, SOIL_FLAG_INVERT_Y);
	}

	m_Texture = m_TextureCache[path];
}

void Texture::Bind()
{	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}

Texture::~Texture()
{	glDeleteTextures(1, &m_Texture);
}


