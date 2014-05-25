#version 430

layout (binding=0) uniform sampler2D DiffuseTexture;
layout (binding=1) uniform sampler2D ShadowTexture;
layout (binding=2) uniform sampler2D NormalMapTexture;
layout (binding=3) uniform sampler2D SpecularMapTexture;

//TerrainTextures
layout (binding=4) uniform sampler2D AsphaltTexture;
layout (binding=5) uniform sampler2D GrassTexture;
layout (binding=6) uniform sampler2D SandTexture;
layout (binding=7) uniform sampler2D BlendMap;

uniform float texScale; //Determines how many times the textures will loop over the terrain
uniform vec3 SunDirection_cameraspace;
uniform mat4 V;

in VertexData
{
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
	vec4 ShadowCoord;
	vec3 Tangent;
	vec3 BiTangent;
} Input;

out vec4 frag_Diffuse;
out vec4 frag_Position;
out vec4 frag_Normal;
out vec4 frag_Specular;

float Shadow(vec4 ShadowCoord)
{
	if (Input.ShadowCoord.x < 0.0 || Input.ShadowCoord.x > 1.0 || Input.ShadowCoord.y < 0.0 || Input.ShadowCoord.y > 1.0)
		return 0.9;
	
	//Fixed bias
	//float bias = 0.00005;

	//Variable bias
	vec3 n = normalize(Input.Normal);
	vec3 l = normalize(SunDirection_cameraspace);
	float cosTheta = clamp(dot(n, l), 0.0, 1.0);
	float bias = tan(acos(cosTheta));
	bias = clamp(bias, 0.0, 0.00005);

	if( texture(ShadowTexture, Input.ShadowCoord.xy).z < ShadowCoord.z + bias)
	{
		return 0.6;
	}
	else 
	{
		return 1.0;
	}
}

void main()
{
	//Fixa så den bara gör detta om modellen har en blend map
	//vvvvvv

	vec4 Blend = texture2D(BlendMap, TextureCoord.st ).rgb;
	vec4 AsphaltTexel = texture2D(AsphaltTexture, TextureCoord.st * texScale).rgb;
	vec4 GrassTexel = texture2D(GrassTexture, TextureCoord.st * texScale).rgb;
	vec4 SandTexel = texture2D(SandTexture, TextureCoord.st * texScale).rgb;

	//Mix the Terrain-textures together
	AsphaltTexel *= mixmap.r;
	GrassTexel = mix(AsphaltTexel, GrassTexel, mixmap.g);
	vec4 tex = mix(GrassTexel, SandTexel, mixmap.b);

	//^^^^^^
	//Fixa så den bara gör detta om modellen har en blend map


	// Diffuse Texture
	frag_Diffuse = tex;
	//frag_Diffuse = texture(DiffuseTexture, Input.TextureCoord) * Shadow(Input.ShadowCoord);

	// G-buffer Position
	frag_Position = vec4(Input.Position.xyz, 1.0);

	// G-buffer Normal
	mat3 TBN = mat3(Input.Tangent, Input.BiTangent, Input.Normal);
	frag_Normal = normalize(vec4(TBN * vec3(texture(NormalMapTexture, Input.TextureCoord)), 0.0));
	//frag_Diffuse = normalize(vec4(TBN * vec3(texture(NormalMapTexture, Input.TextureCoord)), 0.0));
	//frag_Normal = vec4(Input.Normal, 0.0);

	//G-buffer Specular
	frag_Specular = texture(SpecularMapTexture, Input.TextureCoord);
}