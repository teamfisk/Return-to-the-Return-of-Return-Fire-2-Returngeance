#include "PrecompiledHeader.h"
#include "Model.h"

Model::Model(OBJ &obj, ResourceManager* rm)
{
	OBJ::MaterialInfo* currentMaterial = nullptr;
	TextureGroup* currentTexGroup = nullptr;
	int index = 0;
	for (auto face : obj.Faces)
	{
		if (face.Material == nullptr)
		{
			LOG_ERROR("Missing material for .obj file \"%s\"", obj.Path().string().c_str());
			return;
		}
		// New material
		if (face.Material != currentMaterial)
		{
			currentMaterial = face.Material;
			// Load texture
			auto texture = std::shared_ptr<Texture>(rm->Load<Texture>("Texture", currentMaterial->DiffuseTexture.FileName));
			// TODO: Load material parameters
			// Create new texture group (start index of new group is upcoming index)
			TextureGroup texGroup = { texture, index, index };
			TextureGroups.push_back(texGroup);
			currentTexGroup = &TextureGroups.back();
		}

		// Face definitions
		for (auto faceDef : face.Definitions)
		{
			glm::vec3 vertex;
			std::tie(vertex.x, vertex.y, vertex.z) = obj.Vertices.at(faceDef.VertexIndex - 1);
			Vertices.push_back(vertex);

			if (faceDef.NormalIndex != 0)
			{
				glm::vec3 normal;
				std::tie(normal.x, normal.y, normal.z) = obj.Normals.at(faceDef.NormalIndex - 1);
				Normals.push_back(normal);
			}

			if (faceDef.TextureCoordIndex != 0)
			{
				glm::vec2 texCoord;
				// TODO: W-coord?
				std::tie(texCoord.x, texCoord.y, std::ignore) = obj.TextureCoords.at(faceDef.TextureCoordIndex - 1);
				TextureCoords.push_back(texCoord);
			}

			currentTexGroup->EndIndex = index;
			index++;
		}
	}

	if (Vertices.size() > 0)
	{
		CreateBuffers(Vertices, Normals, TextureCoords);
	}
	else
	{
		LOG_WARNING("Loaded OBJ with no vertices!");
	}
}

void Model::CreateBuffers( std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2>textureCoords)
{

	LOG_INFO("Generating VertexBuffer");
	glGenBuffers(1, &VertexBuffer);
	if (vertices.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		GLERROR("GLEW: BufferFail, VertexBuffer");
	}
	else
	{
		LOG_WARNING("Created empty vertex buffer!");
	}

	LOG_INFO("Generating NormalBuffer");
	glGenBuffers(1, &NormalBuffer);
	if (normals.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		GLERROR("GLEW: BufferFail, NormalBuffer");
	}
	else
	{
		LOG_WARNING("Created empty normal buffer!");
	}


	LOG_INFO("Generating textureCoordBuffer");
	glGenBuffers(1, &TextureCoordBuffer);
	if (textureCoords.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, TextureCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2), &textureCoords[0], GL_STATIC_DRAW);
		GLERROR("GLEW: BufferFail, TextureCoordBuffer");
	}
	else
	{
		LOG_WARNING("Created empty texture coordinate buffer!");
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	GLERROR("GLEW: BufferFail4");

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	GLERROR("GLEW: BufferFail5");

	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	GLERROR("GLEW: BufferFail5");

	glBindBuffer(GL_ARRAY_BUFFER, TextureCoordBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	GLERROR("GLEW: BufferFail5");

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	GLERROR("GLEW: BufferFail5");
}


