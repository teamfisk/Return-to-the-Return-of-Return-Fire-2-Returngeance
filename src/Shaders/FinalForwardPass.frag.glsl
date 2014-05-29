#version 430

//uniform vec3 La;

layout (binding=0) uniform sampler2D DiffuseTexture;

in VertexData
{
	vec3 Position;
	vec2 TextureCoord;
} Input;

out vec4 FragmentColor;

void main()
{
	vec4 DiffuseTexel = texture(DiffuseTexture, Input.TextureCoord);
	//vec4 LightingTexel = texture(LightingTexture, Input.TextureCoord);

	//FragmentColor = LightingTexel + vec4(LightingTexel.a, LightingTexel.a, LightingTexel.a, 0.0);
	//FragmentColor = DiffuseTexel;

	FragmentColor = DiffuseTexel;
	//FragmentColor = vec4(pow(_FragmentColor.rgb, vec3(1.0 / Gamma)), _FragmentColor.a);
}