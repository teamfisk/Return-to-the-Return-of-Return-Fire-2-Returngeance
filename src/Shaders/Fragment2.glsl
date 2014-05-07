#version 430

layout (binding=0) uniform sampler2D DiffuseTexture;
layout (binding=1) uniform sampler2D PositionTexture;
layout (binding=2) uniform sampler2D NormalTexture;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 ls;
uniform vec3 ld;
uniform vec3 lp; 
const float specularExponent = 20.0;
uniform vec3 CameraPosition;

const vec3 kd = vec3(1.0, 1.0, 1.0);
const vec3 ks = vec3(1.0, 1.0, 1.0);
const float kshine = 1.0;

in VertexData
{
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
} Input;

out vec4 FragColor;

vec3 phong (vec3 PositionTexel, vec3 NormalTexel)
{
	vec3 lightPosition = vec3( M * vec4( lp, 1.0 ) );
	vec3 distToLight = lightPosition - PositionTexel;
	vec3 directionToLight = normalize(distToLight);

	//Diffuse light
	float dotProdDiffuse = max(dot(directionToLight, NormalTexel), 0.0);
	vec3 Id = ld * kd * dotProdDiffuse; //Final diffuse intensity

	//Specular light
	vec3 reflection = reflect(-directionToLight, NormalTexel);
	vec3 surfaceToCamera = normalize(PositionTexel);
	vec3 HalfWay = normalize(surfaceToCamera + directionToLight);
	float dotProdSpecular = dot(HalfWay, NormalTexel);
	dotProdSpecular = max(dotProdSpecular, 0.0);
	float specularFactor = pow(dotProdSpecular, specularExponent);
	vec3 Is = ls * ks * specularFactor; //Final specular intensity

	//Attenuation
	float dist2D = distance(lightPosition, PositionTexel);
	float attenuationFactor = -log(min(1.0, dist2D / 5.0));

	//vec3 FinalOut = (Id + Is) * attenuationFactor;
	vec3 FinalOut = Id + Is;
	return FinalOut;
}


vec3 phong2 (vec3 PositionTexel, vec3 NormalTexel)
{
	vec3 LightVector = lp - PositionTexel;
	vec3 ViewVector = normalize(PositionTexel);

	//diffuse
	vec3 Id = max(0.0, dot(LightVector, NormalTexel)) * ld;

	vec3 FinalOut = Id;
	return FinalOut;
}

vec3 phong3 (vec3 PositionTexel, vec3 NormalTexel)
{
	vec3 lightDir = lp - PositionTexel;
	lightDir = normalize(lightDir);

	vec3 eyeDir = normalize(CameraPosition-PositionTexel);
	vec3 vHalfVector = normalize(lightDir.xyz+eyeDir);
	vec3 Id = max(0.0, dot(NormalTexel, lightDir)) * ld;
	vec3 Is = pow(max(0.0, dot(NormalTexel, vHalfVector)), 100.0) * ls;
	vec3 FinalFrag = Id + Is;
	return FinalFrag;
}

void main()
{
	vec4 DiffuseTexel = texture2D(DiffuseTexture, Input.TextureCoord);
	vec4 PositionTexel = texture2D(PositionTexture, Input.TextureCoord);
	vec4 NormalTexel = texture2D(NormalTexture, Input.TextureCoord);

	vec4 Frag_color;
	Frag_color.rgb = phong((MVP * PositionTexel).rgb, normalize(NormalTexel).rgb);
	Frag_color.a = 1.0;
	FragColor = Frag_color * DiffuseTexel;
}