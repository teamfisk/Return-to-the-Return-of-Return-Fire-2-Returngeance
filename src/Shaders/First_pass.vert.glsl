#version 130

precision mediump float;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

in vec3 normal;
in vec2 texCoord;
in vec3 vertex;

in float intensity;
in float ambientLight;

out vec3 fragmentNormal;
out vec2 fragmentTexCoord;
out float extIntensity;
out float extAmbientLight;

void main(void)
{
	fragmentTexCoord = texCoord;
	fragmentNormal = normalize((modelMatrix*vec4(normal, 0.0).xyz);
	gl_Position = vec3(modelMatrix * vertex); //Copy position to the fragment shader
	extIntensity = intensity/255.0;
	extAmbientLight = ambientLight/255.0;
}



/*#version 400

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 TextureCoord;

uniform mat4 P, V, M;

out vec3 p_eye;
out vec3 n_eye;

void main () {
  p_eye = (V * M * vec4 (vp, 1.0)).xyz;
  n_eye = (V * M * vec4 (vn, 0.0)).xyz;
  gl_Position = P * vec4 (p_eye, 1.0);
}*/