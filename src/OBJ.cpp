#include "PrecompiledHeader.h"
#include "OBJ.h"

bool OBJ::LoadFromFile(std::string filename)
{
	m_Path = boost::filesystem::path(filename);

	// http://paulbourke.net/dataformats/obj/
	std::ifstream file(m_Path.string());
	if (!file.is_open()) {
		LOG_ERROR("Failed to open .obj \"%s\"", m_Path.string().c_str());
		return false;
	}

	LOG_INFO("Parsing .obj \"%s\"", m_Path.string().c_str());
	
	std::string line;
	while (std::getline(file, line)) {
		if (line.length() == 0)
			continue;

		std::stringstream ss(line);

		std::string prefix;
		ss >> prefix;

		// Ignore comments
		if (prefix == "#")
			continue;

		// Material files
		if (prefix == "mtllib") {
			std::string materialFilename;
			ss >> materialFilename;
			m_MaterialPath = m_Path.branch_path() / materialFilename;
			ParseMaterial();
			continue;
		}

		// Material statement
		if (prefix == "usemtl") {
			std::string material;
			ss >> material;
			m_CurrentMaterial = &Materials[material];
			continue;
		}

		// Vertices
		if (prefix == "v") {
			float x, y, z;
			ss >> x >> y >> z;
			Vertices.push_back(std::make_tuple(x, y, z));
			continue;
		}

		// Normals
		if (prefix == "vn") {
			float x, y, z;
			ss >> x >> y >> z;
			Normals.push_back(std::make_tuple(x, y, z));
		}

		// Texture coordinates
		if (prefix == "vt")	{
			float u, v, w;
			ss >> u >> v >> w;
			TextureCoords.push_back(std::make_tuple(u, v, w));
		}

		// Face definitions
		if (prefix == "f") {
			Face face;
			face.Material = m_CurrentMaterial;

			std::string faceDefString;
			while (ss >> faceDefString) {
				std::stringstream ss2(faceDefString);
				FaceDefinition faceDef = { 0, 0, 0 };

				ss2 >> faceDef.VertexIndex;
				ss2.ignore(); // Ignore first delimiter
				if (!ss2)
					continue;

				if (ss2.peek() == '/') {
					ss2.ignore();
					ss2 >> faceDef.NormalIndex;
				} else {
					ss2 >> faceDef.TextureCoordIndex;
					ss2.ignore();
					ss2 >> faceDef.NormalIndex;
				}
				face.Definitions.push_back(faceDef);
			}
			Faces.push_back(face);
		}
	}

	return true;
}

void OBJ::ParseMaterial()
{
	// http://paulbourke.net/dataformats/mtl/
	std::ifstream file(m_MaterialPath.string());
	if (!file.is_open()) {
		LOG_ERROR("Failed to open .mtl \"%s\"", m_MaterialPath.string().c_str());
		return;
	}

	LOG_INFO("Parsing .mtl \"%s\"", m_MaterialPath.string().c_str());

	std::string currentMaterialName;
	MaterialInfo* currentMaterial = nullptr;

	std::string line;
	while (std::getline(file, line)) {
		if (line.length() == 0)
			continue;

		std::stringstream ss(line);

		std::string prefix;
		ss >> prefix;

		// Create a new material definition
		if (prefix == "newmtl")	{
			MaterialInfo mat = 
			{
				"",
				std::make_tuple(0.2f, 0.2f, 0.2f),
				std::make_tuple(0.8f, 0.8f, 0.8f),
				std::make_tuple(1.0f, 1.0f, 1.0f),
				std::make_tuple(1.0f, 1.0f, 1.0f),
				1.0f,
				1.0f,
				0.0f,
				0,
			};

			ss >> currentMaterialName;
			LOG_INFO("Parsing material %s", currentMaterialName.c_str());
			Materials[currentMaterialName] = mat;
			currentMaterial = &Materials[currentMaterialName];
			continue;
		}

		if (!currentMaterial)
			continue;

		// Ambient color
		if (prefix == "Ka")	{
			float r, g, b;
			ss >> r >> g >> b;
			currentMaterial->AmbientColor = std::make_tuple(r, g, b);
			continue;
		}
		// Diffuse color
		if (prefix == "Kd")	{
			float r, g, b;
			ss >> r >> g >> b;
			currentMaterial->DiffuseColor = std::make_tuple(r, g, b);
			continue;
		}
		// Specular color
		if (prefix == "Ks")	{
			float r, g, b;
			ss >> r >> g >> b;
			currentMaterial->SpecularColor = std::make_tuple(r, g, b);
			continue;
		}
		// Transmission filter
		if (prefix == "Tf")	{
			std::stringstream ss2;
			ss2 << ss.str();

			std::string command;
			ss2 >> command;
			if (command == "xyz") {
				// TODO: "The "Ks xyz" statement specifies the specular reflectivity using CIEXYZ values."
			}
			else if (command == "spectral")	{
				// TODO: "The "Tf spectral" statement specifies the transmission filter using a spectral curve."
			} else {
				float r, g, b;
				ss >> r;
				// G and B are optional
				if (!(ss >> g >> b))
				{
					g = r;
					b = r;
				}
				currentMaterial->TransmissionFilter = std::make_tuple(r, g, b);
			}
			continue;
		}
		// Optical density
		if (prefix == "Ni")	{
			ss >> currentMaterial->OpticalDensity;
			continue;
		}
		// Alpha
		if (prefix == "d" || prefix == "Tr") {
			ss >> currentMaterial->Alpha;
			continue;
		}
		// Shininess
		if (prefix == "Ns")	{
			ss >> currentMaterial->Shininess;
			continue;
		}
		// Illumination model
		if (prefix == "illum") {
			int illum = 0;
			ss >> illum;
			currentMaterial->IlluminationModel = illum;
			continue;
		}
		// Texture file
		// TODO:
		if (prefix == "map_Ka" || prefix == "map_Kd") {
			std::string textureFile;
			ss >> textureFile;
			currentMaterial->TextureFile = (m_MaterialPath.branch_path() / textureFile).string();
			continue;
		}
	}
}
