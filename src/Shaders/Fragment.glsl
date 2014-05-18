#version 430

layout (binding=0) uniform sampler2D DiffuseTexture;
layout (binding=1) uniform sampler2D ShadowTexture;
layout (binding=2) uniform sampler2D NormalMapTexture;
layout (binding=3) uniform sampler2D SpecularMapTexture;


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
out vec4 frag_specular;

float Shadow(vec4 ShadowCoord)
{
	//float cosTheta = clamp(dot(Input.Normal, 1.0), 0.0, 1.0);
	float bias = 0.0005; // cosTheta is dot( n,l ), clamped between 0 and 1
	bias = clamp(bias, 0.0, 0.01);
	if( texture(ShadowTexture, Input.ShadowCoord.xy).z < ShadowCoord.z - bias)
	{
		return 0.3;
	}
	else 
	{
		return 1.0;
	}
}

void main()
{
	
	// Diffuse Texture
	frag_Diffuse = texture(DiffuseTexture, Input.TextureCoord) * Shadow(Input.ShadowCoord);

	// G-buffer Position
	frag_Position = vec4(Input.Position.xyz, 1.0);

	// G-buffer Normal
	mat3 TBN = transpose(mat3(Input.Tangent, Input.BiTangent, Input.Normal));
	frag_Normal = normalize(vec4(TBN * vec3(texture(NormalMapTexture, Input.TextureCoord)), 0.0));
	//frag_Normal = vec4(Input.Normal, 0.0);

	//G-buffer Specular
	frag_specular = texture(SpecularMapTexture, Input.TextureCoord);
}