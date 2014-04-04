#version 430

uniform mat4 MVP;
uniform mat4 DepthMVP;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TextureCoord;

out VertexData {
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
	vec3 ShadowCoord;
} Output;

void main()
{
	gl_Position = MVP * vec4(Position, 1.0);

	Output.Position = Position;
	Output.Normal = Normal;
	Output.TextureCoord = TextureCoord;
	Output.ShadowCoord = vec3(DepthMVP * vec4(Position, 1.0));
}