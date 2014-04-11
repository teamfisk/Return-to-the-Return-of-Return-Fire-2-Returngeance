#version 430

uniform mat4 MVP;

layout(location = 0) in vec3 Position;

out VertexData
{
	vec3 TextureCoord;
} Output;

void main()
{
	gl_Position = MVP * vec4(Position, 1.0);
	Output.TextureCoord = Position;
}