#version 430

uniform vec3 La;
uniform float Gamma;

layout (binding=0) uniform sampler2D DiffuseTexture;
layout (binding=1) uniform sampler2D LightingTexture;

in VertexData
{
	vec3 Position;
	vec2 TextureCoord;
} Input;

out vec4 FragmentColor;

void main()
{
	vec4 DiffuseTexel = texture(DiffuseTexture, Input.TextureCoord);
	vec4 LightingTexel = texture(LightingTexture, Input.TextureCoord);

	vec4 _FragmentColor = DiffuseTexel * vec4(La, 1.0) + LightingTexel;
	FragmentColor = vec4(pow(_FragmentColor.rgb, vec3(1.0 / Gamma)), _FragmentColor.a);
}