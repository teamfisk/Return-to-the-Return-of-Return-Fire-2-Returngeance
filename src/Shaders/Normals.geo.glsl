#version 430

uniform mat4 MVP;

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VertexData
{	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
} Input[3];

out VertexData
{	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
} Output;

void main()
{	for (int i = 0; i < gl_in.length(); i++)
	{	gl_Position = MVP * vec4(Input[i].Position, 1.0);
		EmitVertex();
		gl_Position = MVP * vec4(Input[i].Position + Input[i].Normal, 1.0);
		EmitVertex();
		EndPrimitive();

		Output.Position = Input[i].Position;
		Output.Normal = Input[i].Normal;
		Output.TextureCoord = Input[i].TextureCoord;
	}
}