#version 430

layout(binding = 0) uniform sampler2D DepthTexture;

in VertexData
{
	vec3 Position;
	vec2 TextureCoord;
} Input;

out vec4 FragmentColor;

float LinearizeDepth(float z)
{
	float n = 0.1; // camera z near
	float f = 800.0; // camera z far
	return (2.0 * n) / (f + n - z * (f - n));
}

void main()
{
	float z = texture(DepthTexture, Input.TextureCoord).x;
	vec4 color = vec4(z, z, z, 0);

	FragmentColor = color;
}