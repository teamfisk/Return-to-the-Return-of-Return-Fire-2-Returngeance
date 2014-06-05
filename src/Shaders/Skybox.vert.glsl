#version 430

uniform mat4 V;
uniform mat4 P;

layout(location = 0) in vec3 Position;

out VertexData
{
	vec3 TextureCoord;
} Output;

void main()
{
	vec4 pos = V * vec4(Position, 1.0);
	gl_Position = P * pos;
	Output.TextureCoord = Position;
}