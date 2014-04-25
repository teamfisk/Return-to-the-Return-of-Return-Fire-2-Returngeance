#version 130
uniform sampler2D firstTexture;
in vec3 fragmentNormal;
in vec2 fragmentTexCoord;
in vec3 position;
layout (location = 0) out vec4 diffuseOutput; 
layout (location = 1) out vec4 posOutput;     
layout (location = 2) out vec4 normOutput;    
layout (location = 3) out vec4 blendOutput;   

void main(void)
{
	posOutput.xyz = position;
	normOutPut = vec4(fragmentNormal, 0);
	vec4 clr = texture(firstTexture, fragmentTexCoord);
	float alpha = clr.a;
	if(alpha < 0.1)
		discard; //Some optimizing
	blendOutput.rgb = clr.rgb * clr.a; //Pre multiplied alpha
	blendOutput.a = clr.a;
	diffuseOutput = clr;
}

/*#version 400

in vec3 p_eye;
in vec3 n_eye;

layout (location = 0) out vec4 def_p; // "go to GL_COLOR_ATTACHMENT0"
layout (location = 1) out vec4 def_n; // "go to GL_COLOR_ATTACHMENT1"

void main () {
  def_p = vec4(p_eye, 1.0);
  def_n = vec4(n_eye, 1.0);
}*/