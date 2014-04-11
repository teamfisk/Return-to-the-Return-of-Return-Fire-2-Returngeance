#ifndef OBJ_h__
#define OBJ_h__

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <map>

#include <boost/filesystem/path.hpp>

class OBJ
{
public:
	struct MaterialInfo
	{	std::string TextureFile;
		std::tuple<float, float, float> AmbientColor;
		std::tuple<float, float, float> DiffuseColor;
		std::tuple<float, float, float> SpecularColor;
		std::tuple<float, float, float> TransmissionFilter;
		float OpticalDensity;
		float Alpha;
		float Shininess;
		int IlluminationModel;
	};

	struct FaceDefinition
	{	int VertexIndex;
		int TextureCoordIndex;
		int NormalIndex;
	};

	struct Face
	{	Face() : Material(nullptr) { }
		std::vector<FaceDefinition> Definitions;
		MaterialInfo* Material;
	};

	OBJ() : m_CurrentMaterial(nullptr) { }
	OBJ(std::string filename) : m_CurrentMaterial(nullptr) { LoadFromFile(filename); }

	std::vector<std::tuple<float, float, float>> Vertices;
	std::vector<std::tuple<float, float, float>> Normals;
	std::vector<std::tuple<float, float, float>> TextureCoords;
	std::vector<Face> Faces;
	std::map<std::string, MaterialInfo> Materials;

	bool LoadFromFile(std::string filename);
	boost::filesystem::path Path() const { return m_Path; }

private:
	boost::filesystem::path m_Path;
	boost::filesystem::path m_MaterialPath;
	MaterialInfo* m_CurrentMaterial;

	void ParseMaterial();
};

#endif // OBJ_h__