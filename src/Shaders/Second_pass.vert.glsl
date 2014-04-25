#version 130
in vec4 vertex; out vec2 position;

void main(void)
{
	gl_Position = vertex*2-1;
	gl_Position.z = 0.0;
	position = vertex.xy;
}