#version 430

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

layout (binding=0) uniform sampler2D DiffuseTexture;
layout (binding=1) uniform sampler2D NormalMap;

in VertexData
{
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
} Input;

out vec4 frag_Diffuse;
out vec4 frag_Position;
out vec4 frag_Normal;

void main()
{
	// Diffuse Texture
	frag_Diffuse = texture2D(DiffuseTexture, Input.TextureCoord);

	// G-buffer Position
	frag_Position = vec4(Input.Position.xyz, 0.0);

	// G-buffer Normal
	frag_Normal = vec4(normalize(vec3(Input.Normal * (texture2D(NormalMap, Input.TextureCoord)).xyz)), 0.0);
}