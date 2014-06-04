#version 430

layout (binding=0) uniform sampler2D PositionTexture;
layout (binding=1) uniform sampler2D NormalsTexture;
layout (binding=2) uniform sampler2D SpecularTexture;


uniform vec2 ViewportSize;
uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 CameraPosition;
uniform vec3 directionToSun;

const vec3 ks = vec3(1.0, 1.0, 1.0);
const vec3 kd = vec3(1.0, 1.0, 1.0);
const vec3 ka = vec3(1.0, 1.0, 1.0);
const float kshine = 1.0;
const vec3 SunDiffuseLight = vec3(1.0, 1.0, 1.0);
const vec3 SunSpecularLight = vec3(1.0, 1.0, 1.0);
const vec3 SunPos = directionToSun*vec3(100);
const float specularExponent = 100;
const vec3 La = vec3(0.3, 0.3, 0.3);



in VertexData
{
	vec3 Position;
	vec2 TextureCoord;
} Input;

out vec4 frag_Light;

vec4 phong(vec3 position, vec3 normal, vec3 specular)
{
	
	//Diffuse Sunlight
	vec3 directionToLight = normalize(vec3(V * vec4(directionToSun, 0.0)));
	float dotProdLight = dot(directionToLight, normal);
	dotProdLight = max(dotProdLight, 0.0);
	vec3 sId = kd * SunDiffuseLight * dotProdLight;

	//Specular Sunlight
	vec3 surfaceToViewer = normalize(-position);
	vec3 halfWay = normalize(surfaceToViewer + directionToLight);
	float dotSpecular = max(dot(halfWay, normal), 0.0);
	float specularFactorSun = pow(dotSpecular, specularExponent);
	vec3 sIs = specular.r * SunSpecularLight * specularFactorSun;

	return vec4((sId), sIs.r);
}

void main()
{
	vec2 TextureCoord = gl_FragCoord.xy / ViewportSize;
	vec4 PositionTexel = texture(PositionTexture, TextureCoord);
	vec4 NormalTexel = texture(NormalsTexture, TextureCoord);
	vec4 SpecularTexel = texture(SpecularTexture, TextureCoord);


	frag_Light = phong(vec3(PositionTexel), vec3(NormalTexel), vec3(SpecularTexel));
	//FragColor = DiffuseTexel * (vec4(La, 0.0) * (vec4(LightTexel.rgb, 0.0) + vec4(LightTexel.a, LightTexel.a, LightTexel.a, 0.0)));
	//FragColor = NormalTexel;
}