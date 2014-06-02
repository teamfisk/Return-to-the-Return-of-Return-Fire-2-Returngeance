#version 430

uniform vec4 Color;

layout(binding=0) uniform sampler2D texture0;

in VertexData {
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
} Input;
 
out vec4 frag_Diffuse;

void main() {
	// Texture
	vec4 texel = texture(texture0, Input.TextureCoord);

	frag_Diffuse = texel * Color;
}