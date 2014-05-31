#version 430

uniform samplerCube CubemapTexture;

in VertexData
{
	vec3 TextureCoord;
} Input;

out vec4 FragColor;

void main()
{
	FragColor = texture(CubemapTexture, Input.TextureCoord);
}