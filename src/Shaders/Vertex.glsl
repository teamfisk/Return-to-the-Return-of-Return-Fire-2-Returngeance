#version 430

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TextureCoord;

out VertexData
{
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
} Output;

void main()
{
	gl_Position = MVP * vec4(Position, 1.0);

	Output.Position = vec3(V * M * vec4(Position, 1.0));
	Output.Normal = normalize(vec3(inverse(transpose(V * M)) * vec4(Normal, 0.0)));
	Output.TextureCoord = TextureCoord;
}