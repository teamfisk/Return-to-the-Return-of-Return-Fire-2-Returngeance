#version 430

layout (location = 0) in vec3 Position;
layout (location = 2) in vec2 TextureCoord;

out VertexData
{
	vec3 Position;
	vec2 TextureCoord;
} Output;

void main()
{
	gl_Position = vec4(Position, 1.0);
	Output.Position = Position;
	Output.TextureCoord = TextureCoord;
}