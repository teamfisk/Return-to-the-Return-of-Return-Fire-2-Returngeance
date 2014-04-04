#ifndef Texture_h__
#define Texture_h__

#include <string>

#include <SOIL.h>

class Texture
{
public:
	Texture(std::string path);
	
	~Texture();

	GLuint texture;
	
	void Load(std::string path);
	void Bind();
};

#endif // Texture_h__
