#include "PrecompiledHeader.h"
#include "Model.h"

Model::Model(const char* path)
{
	Loadobj(path, Vertices, Normals, TextureCoords);
	CreateBuffers(Vertices, Normals, TextureCoords);
}

Model::Model(OBJ &obj)
{
	OBJ::MaterialInfo* currentMaterial = nullptr;
	TextureGroup* currentTexGroup = nullptr;
	int index = 0;
	for (auto face : obj.Faces) {
		if (face.Material == nullptr) {
			LOG_ERROR("Missing material for .obj file \"%s\"", obj.Path().string().c_str());
			return;
		} 
		// New material
		if (face.Material != currentMaterial) {
			currentMaterial = face.Material;
			// Load texture
			std::shared_ptr<Texture> texture = std::make_shared<Texture>(currentMaterial->TextureFile);
			// TODO: Load material parameters
			// Create new texture group (start index of new group is upcoming index)
			TextureGroup texGroup = { texture, index, index };
			TextureGroups.push_back(texGroup);
			currentTexGroup = &TextureGroups.back();
		}

		// Face definitions
		for (auto faceDef : face.Definitions) {
			glm::vec3 vertex;
			std::tie(vertex.x, vertex.y, vertex.z) = obj.Vertices.at(faceDef.VertexIndex - 1);
			Vertices.push_back(vertex);

			if (faceDef.NormalIndex != 0) {
				glm::vec3 normal;
				std::tie(normal.x, normal.y, normal.z) = obj.Normals.at(faceDef.NormalIndex - 1);
				Normals.push_back(normal);
			}

			if (faceDef.TextureCoordIndex != 0) {
				glm::vec2 texCoord;
				// TODO: W-coord?
				std::tie(texCoord.x, texCoord.y, std::ignore) = obj.TextureCoords.at(faceDef.TextureCoordIndex - 1);
				TextureCoords.push_back(texCoord);
			}

			currentTexGroup->EndIndex = index;
			index++;
		}
	}

	if (Vertices.size() > 0) {
		CreateBuffers(Vertices, Normals, TextureCoords);
	}
}

bool Model::Loadobj(const char* path, std::vector <glm::vec3> & out_vertices, std::vector <glm::vec3> &out_normals, std::vector <glm::vec2> & out_TextureCoords)
{
	std::vector< unsigned int > vertexIndices, TextureCoordIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_TextureCoords;
	std::vector< glm::vec3 > temp_normals;

	FILE * file = fopen(path, "r");
	LOG_INFO("Loading .obj file");
	if( file == NULL )
	{
		LOG_INFO("Load .obj file: failed");
		return false;
	}
	char lineHeader[512];

	while(true)
	{
		
		//read the first word of the line
		int res = fscanf(file, "%s", lineHeader);

		if( res == EOF ) // EOF - End Of File
		{
			for( unsigned int i = 0; i < vertexIndices.size(); i++ )
			{
				unsigned int vertexIndex = vertexIndices[i];
				glm::vec3 vertex = temp_vertices[ vertexIndex-1];
				out_vertices.push_back(vertex);

			}

			for( unsigned int i = 0; i < TextureCoordIndices.size(); i++ )
			{
				unsigned int TextureCoordIndex = TextureCoordIndices[i];
				glm::vec2 TextureCoord = temp_TextureCoords[ TextureCoordIndex-1];
				out_TextureCoords.push_back(TextureCoord);
			}

			for( unsigned int i = 0; i < normalIndices.size(); i++ )
			{
				unsigned int normalIndex = normalIndices[i];
				glm::vec3 normal = temp_normals[ normalIndex-1];
				out_normals.push_back(normal);
			}


			LOG_INFO("Model Loaded\n");
			break;
		}

		if( strcmp( lineHeader, "v" ) == 0 ) // vertex
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 ) // texture coordinate
		{
			glm::vec2 TextureCoord;
			fscanf(file, "%f %f\n", &TextureCoord.x, &TextureCoord.y );
			temp_TextureCoords.push_back(TextureCoord);
		}
		else if( strcmp( lineHeader, "vn" ) == 0 ) // normal
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}
		else if( strcmp( lineHeader, "f" ) == 0)
		{
			unsigned int vertexIndex[3], TextureCoordIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &TextureCoordIndex[0], &normalIndex[0], &vertexIndex[1], &TextureCoordIndex[1], &normalIndex[1],&vertexIndex[2], &TextureCoordIndex[2], &normalIndex[2]);
			if(matches != 9)
			{
				printf("File can't be read, try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			TextureCoordIndices.push_back(TextureCoordIndex[0]);
			TextureCoordIndices.push_back(TextureCoordIndex[1]);
			TextureCoordIndices.push_back(TextureCoordIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

		}
		else if ( strcmp( lineHeader, "mtllib" ) == 0 ) 
		{

			char fileName[512];
			fscanf(file, "%s\n", &fileName);


			FILE * mtlfile = fopen(fileName, "r");
			LOG_INFO("Loading .mtl file");
			if( mtlfile == NULL )
			{
				LOG_INFO("Load .mtl file: failed");
				return false;
			}

			char mtllineHeader[512];
			//read the first word of the line

			while (true)
			{
				int mtlres = fscanf(mtlfile, "%s", mtllineHeader);

				if( mtlres == EOF ) // EOF - End Of File
				{
					
					break;
				}
				else if ( strcmp( mtllineHeader, "map_Kd" ) == 0 ) 
				{
					char textureFileName[512];
					fscanf(mtlfile, "%s", textureFileName);
					texture.push_back(std::make_shared<Texture>(textureFileName));
					LOG_INFO("Texture Loaded\n");
				}
			}
			


		}
	}
	return true;

}


void Model::CreateBuffers( std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2>textureCoords)
{

	LOG_INFO("Generating VertexBuffer");
	glGenBuffers(1, &VertexBuffer);
	if (vertices.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		GLERROR("GLEW: BufferFail, VertexBuffer");
	} else {
		LOG_WARNING("Created empty vertex buffer!");
	}

	LOG_INFO("Generating NormalBuffer");
	glGenBuffers(1, &NormalBuffer);
	if (normals.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		GLERROR("GLEW: BufferFail, NormalBuffer");
	} else {
		LOG_WARNING("Created empty normal buffer!");
	}


	LOG_INFO("Generating textureCoordBuffer");
	glGenBuffers(1, &TextureCoordBuffer);
	if (textureCoords.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, TextureCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2), &textureCoords[0], GL_STATIC_DRAW);
		GLERROR("GLEW: BufferFail, TextureCoordBuffer");
	} else {
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


