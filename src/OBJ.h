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
#include <boost/program_options.hpp>

#include "ResourceManager.h"

class OBJ : public Resource
{
public:
	struct MaterialInfo
	{
		struct TextureMap
		{
			TextureMap()
				: blendu(true)
				, blendv(true)
				, clamp(false)
				, o(std::make_tuple(0.f, 0.f, 0.f))
				, s(std::make_tuple(1.f, 1.f, 1.f)) { }

			std::string FileName;

			// http://paulbourke.net/dataformats/mtl/
			// "These options are described in detail in 'Options for texture map statements' on page 5-18."

			// Horizontal texture blending
			bool blendu; // = true
			// Vertical texture blending
			bool blendv; // = true
			// Clamping
			bool clamp; // = false
			// Offset
			std::tuple<float, float, float> o; // = (0, 0, 0)
			// Scale
			std::tuple<float, float, float> s; // = (1, 1, 1)
		};

		struct ColorMap : public TextureMap
		{
			ColorMap()
				: cc(false) { }

			// Color correction
			bool cc; // = false
		};

		struct BumpMap : public TextureMap
		{
			BumpMap()
				: bm(1.f) { }

			// Bump multiplier
			float bm; // = 1?
		};

		MaterialInfo()
			: AmbientColor(std::make_tuple(0.2f, 0.2f, 0.2f))
			, DiffuseColor(std::make_tuple(0.8f, 0.8f, 0.8f))
			, SpecularColor(std::make_tuple(1.0f, 1.0f, 1.0f))
			, TransmissionFilter(std::make_tuple(1.0f, 1.0f, 1.0f))
			, OpticalDensity(1.0f)
			, Alpha(1.0f)
			, Shininess(0.0f)
			, IlluminationModel(0) { }

		ColorMap DiffuseTexture;
		ColorMap SpecularMap;
		BumpMap NormalMap;
		std::tuple<float, float, float> AmbientColor; // = (0.2, 0.2, 0.2)
		std::tuple<float, float, float> DiffuseColor; // = (0.8, 0.8, 0.8)
		std::tuple<float, float, float> SpecularColor; // = (1, 1, 1)
		std::tuple<float, float, float> TransmissionFilter; // = (1, 1, 1)
		float OpticalDensity; // = 1
		float Alpha; // = 1
		float Shininess; // = 0
		int IlluminationModel; // = 0
	};

	struct FaceDefinition
	{
		int VertexIndex;
		int TextureCoordIndex;
		int NormalIndex;
	};

	struct Face
	{
		Face() : Material(nullptr) { }
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
	void ParseTextureMap(unsigned int line, std::stringstream &ss, std::string prefix, std::string arg, MaterialInfo::TextureMap &textureMap);
	void ParseColorMap(unsigned int line, std::stringstream &ss, std::string prefix, std::string arg, MaterialInfo::ColorMap &textureMap);
	void ParseBumpMap(unsigned int line, std::stringstream &ss, std::string prefix, std::string arg, MaterialInfo::BumpMap &textureMap);
};

#endif // OBJ_h__