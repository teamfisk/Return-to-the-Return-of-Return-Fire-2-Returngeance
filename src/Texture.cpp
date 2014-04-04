#include "PrecompiledHeader.h"
#include "Texture.h"

Texture::Texture(std::string path)
{
	Load(path);
}


void Texture::Load(std::string path)
{
	texture = SOIL_load_OGL_texture(path.c_str(), 0, 0, SOIL_FLAG_INVERT_Y);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}


