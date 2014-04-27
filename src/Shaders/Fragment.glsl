#version 430

layout (binding=0) uniform sampler2D DiffuseTexture;

in VertexData
{
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
} Input;

out vec4 frag_Diffuse;
out vec4 frag_Position;
out vec4 frag_Normal;

void main()
{
	// Diffuse Texture
	frag_Diffuse = texture2D(DiffuseTexture, Input.TextureCoord);

	// G-buffer Position
	frag_Position = vec4(Input.Position.xy, 0.0, 0.0);

	// G-buffer Normal
	frag_Normal = vec4(Input.Normal, 0.0);
}