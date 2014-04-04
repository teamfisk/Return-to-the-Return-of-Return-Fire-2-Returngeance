#version 430

uniform vec4 Color;

in VertexData {
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
	vec3 ShadowCoord;
} Input;

out vec4 FragmentColor;

void main() {
	//FragmentColor = vec4(1.0 - gl_Color.r, 1.0 - gl_Color.g, 1.0 - gl_Color.b, 0.0);
	FragmentColor = Color;
}