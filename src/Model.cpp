#include "PrecompiledHeader.h"
#include "Model.h"

Model::Model(std::shared_ptr<ResourceManager> rm, OBJ &obj)
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
			// TODO: Load normal map
			std::shared_ptr<Texture> normalMap = nullptr;
			if (!currentMaterial->NormalMap.FileName.empty())
			{
				normalMap = std::shared_ptr<Texture>(rm->Load<Texture>("Texture", currentMaterial->NormalMap.FileName));
			}
			else
			{
				normalMap = std::shared_ptr<Texture>(rm->Load<Texture>("Texture", "Textures/NeutralNormalMap.png"));
			}
			// Load specular map
			std::shared_ptr<Texture> specularMap = nullptr;
			if (!currentMaterial->SpecularMap.FileName.empty())
			{
				specularMap = std::shared_ptr<Texture>(rm->Load<Texture>("Texture", currentMaterial->SpecularMap.FileName));
			}
			else
			{
				specularMap = std::shared_ptr<Texture>(rm->Load<Texture>("Texture", "Textures/NeutralSpecularMap.png"));
			}

			// TODO: Load material parameters
			// Create new texture group (start index of new group is upcoming index)
			TextureGroup texGroup = { texture, normalMap, specularMap, index, index };
			TextureGroups.push_back(texGroup);
			currentTexGroup = &TextureGroups.back();
		}

		/*std::unordered_map<int, glm::vec3> similarNormals;
		std::unordered_map<int, int> normalCount;
		for (auto &faceDef : face.Definitions)
		{
			if (faceDef.NormalIndex == 0)
				continue;


			similarNormals[faceDef.VertexIndex - 1] += normal;
			normalCount[faceDef.VertexIndex - 1]++;
			int index = pair.first;
			glm::vec3 averagedNormal = ;
			Normals[]
		}*/

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
		CreateTangents();
		//getSimilarVertexIndex();
		CreateBuffers(Vertices, Normals, TangentNormals, BiTangentNormals, TextureCoords);
	}
	else
	{
		LOG_WARNING("Loaded OBJ with no vertices!");
	}
}

void Model::CreateBuffers( std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec3> tangents, std::vector<glm::vec3> biTangents, std::vector<glm::vec2>textureCoords)
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

	LOG_INFO("Generating TangentNormalsBuffer");
	glGenBuffers(1, &TangentNormalsBuffer);
	if (tangents.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, TangentNormalsBuffer);
		glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);
		GLERROR("GLEW: BufferFail, TangentNormalsBuffer");
	}
	else
	{
		LOG_WARNING("Created empty tangent buffer!");
	}

	LOG_INFO("Generating BiTangentNormalsBuffer");
	glGenBuffers(1, &BiTangentNormalsBuffer);
	if (biTangents.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, BiTangentNormalsBuffer);
		glBufferData(GL_ARRAY_BUFFER, biTangents.size() * sizeof(glm::vec3), &biTangents[0], GL_STATIC_DRAW);
		GLERROR("GLEW: BufferFail, BiTangentNormalsBuffer");
	}
	else
	{
		LOG_WARNING("Created empty biTangent buffer!");
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

	glBindBuffer(GL_ARRAY_BUFFER, TangentNormalsBuffer);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	GLERROR("GLEW: BufferFail5");

	glBindBuffer(GL_ARRAY_BUFFER, BiTangentNormalsBuffer);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
	GLERROR("GLEW: BufferFail5");

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	GLERROR("GLEW: BufferFail5");
}

bool Model::IsNear( float v1, float v2 )
{
	return fabs(v1 - v2) < 0.001f;
}

void Model::getSimilarVertexIndex()
{
	for(int i = 0; i < Vertices.size(); i++)
	{
		for(int t = 0; t < Vertices.size(); t++)
		{
			if(i != t)
			{
				if(IsNear(Vertices[i].x, Vertices[t].x)
					&& IsNear(Vertices[i].y, Vertices[t].y)
					&& IsNear(Vertices[i].z, Vertices[t].z)
					)
				{
					glm::vec3 tempNormal, tempTangent, tempBiTangent;

					tempNormal = Normals[i] + Normals[t];
					tempTangent = TangentNormals[i] + TangentNormals[t];
					tempBiTangent = BiTangentNormals[i] + BiTangentNormals[t];

					Normals[i] = tempNormal;
					Normals[t] = tempNormal;

					TangentNormals[i] = tempTangent;
					TangentNormals[t] = tempTangent;

					BiTangentNormals[i] = tempBiTangent;
					BiTangentNormals[t] = tempBiTangent;
				}
			}
		}
	}
}

void Model::CreateTangents()
{
	for(int i = 0; i < Vertices.size(); i += 3)
	{
		glm::vec3 v0 = Vertices[i];
		glm::vec3 v1 = Vertices[i+1];
		glm::vec3 v2 = Vertices[i+2];

		glm::vec2 uv0 = TextureCoords[i];
		glm::vec2 uv1 = TextureCoords[i+1];
		glm::vec2 uv2 = TextureCoords[i+2];

		//Calculate the edge of the triangle
		glm::vec3 edge1 = v1-v0;
		glm::vec3 edge2 = v2-v0;

		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent, biTangent;

		tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * r;
		biTangent = (edge2 * deltaUV1.x - edge1 * deltaUV2.x) * r;

		TangentNormals.push_back(tangent);
		TangentNormals.push_back(tangent);
		TangentNormals.push_back(tangent);

		BiTangentNormals.push_back(biTangent);
		BiTangentNormals.push_back(biTangent);
		BiTangentNormals.push_back(biTangent);
	}
}



