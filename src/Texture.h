#ifndef Texture_h__
#define Texture_h__

#include <string>
#include <unordered_map>

#include <SOIL.h>

#include "ResourceManager.h"

class Texture : public Resource
{
public:
	Texture(std::string path);
	~Texture();

	void Load(std::string path);
	void Bind();

	operator GLuint() const { return m_Texture; }

private:
	GLuint m_Texture;
	std::unordered_map<std::string, GLuint> m_TextureCache;
};


#endif // Texture_h__
