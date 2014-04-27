#version 430

layout (binding=0) uniform sampler2D DiffuseTexture;
layout (binding=1) uniform sampler2D PositionTexture;
layout (binding=2) uniform sampler2D NormalTexture;

in VertexData
{
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
} Input;

out vec4 FragColor;

void main()
{
	FragColor = texture2D(DiffuseTexture, Input.TextureCoord);
}