#version 430

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 DepthMVP;

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TextureCoord;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 BiTangent;

out VertexData
{
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
	vec4 ShadowCoord;
	vec3 Tangent;
	vec3 BiTangent;
} Output;

void main()
{
	gl_Position = MVP * vec4(Position, 1.0);

	Output.Position = vec3(V * M * vec4(Position, 1.0));
	Output.Normal = normalize(vec3(inverse(transpose(V * M)) * vec4(Normal, 0.0)));
	Output.TextureCoord = TextureCoord;
	Output.ShadowCoord = DepthMVP * vec4(Position, 1.0);
	Output.Tangent = normalize(vec3(inverse(transpose(V * M)) * vec4(Tangent, 0.0)));
	Output.BiTangent = normalize(vec3(inverse(transpose(V * M)) * vec4(BiTangent, 0.0)));
}