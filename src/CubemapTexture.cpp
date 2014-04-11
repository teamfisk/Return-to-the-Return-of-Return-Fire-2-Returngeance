#include "PrecompiledHeader.h"
#include "CubemapTexture.h"

CubemapTexture::CubemapTexture(std::string posXFile, std::string negXFile, std::string posYFile, std::string negYFile, std::string posZFile, std::string negZFile)
{	m_Loaded = false;
	m_Texture = 0;
	m_TextureFiles[0] = posXFile;
	m_TextureFiles[1] = negXFile;
	m_TextureFiles[2] = posYFile;
	m_TextureFiles[3] = negYFile;
	m_TextureFiles[4] = posZFile;
	m_TextureFiles[5] = negZFile;
}

CubemapTexture::~CubemapTexture()
{	if (m_Texture != 0)
	{	//glDeleteTextures(1, &m_Texture);
	}
}

void CubemapTexture::Load()
{	m_Loaded = true;

	m_Texture = SOIL_load_OGL_cubemap(
	                m_TextureFiles[0].c_str(),
	                m_TextureFiles[1].c_str(),
	                m_TextureFiles[2].c_str(),
	                m_TextureFiles[3].c_str(),
	                m_TextureFiles[4].c_str(),
	                m_TextureFiles[5].c_str(),
	                SOIL_LOAD_AUTO,
	                SOIL_CREATE_NEW_ID,
	                0);

	if (m_Texture == 0)
	{	LOG_ERROR("SOIL cubemap loading error: %s", SOIL_last_result());
		return;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubemapTexture::Bind(GLenum textureUnit)
{	if (!m_Loaded)
	{	LOG_WARNING("Cubemap \"%s\" was not loaded before being bound! Attempting to load now...", m_TextureFiles[0].c_str());
		Load();
	}

	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
}