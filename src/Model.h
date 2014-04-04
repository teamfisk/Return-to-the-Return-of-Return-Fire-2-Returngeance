#ifndef Model_h__
#define Model_h__

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <stack>

#include "Texture.h"
#include "OBJ.h"

class Model
{
public:
	Model(OBJ &obj);
	Model(const char* path);

	struct TextureGroup
	{
		std::shared_ptr<Texture> Texture;
		unsigned int StartIndex;
		unsigned int EndIndex;
	};

	GLuint VAO;
	std::vector<TextureGroup> TextureGroups;

	std::vector<std::shared_ptr<Texture>> texture;
	glm::mat4 GetMatrix();
	std::vector<glm::vec3> Vertices;

private:
	
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> TextureCoords;

	GLuint VertexBuffer;
	GLuint NormalBuffer;
	GLuint TextureCoordBuffer;

	bool Loadobj(
		const char* path, 
		std::vector <glm::vec3> & out_vertices, 
		std::vector <glm::vec3> &out_normals, 
		std::vector <glm::vec2> & out_TextureCoords
		);

	void CreateBuffers(
		std::vector<glm::vec3> _Vertices, 
		std::vector<glm::vec3> _Normals, 
		std::vector<glm::vec2>_TextureCoords
		);

};
#endif // Model_h__