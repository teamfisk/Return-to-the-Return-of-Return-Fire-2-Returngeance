#version 430

uniform mat4 model;
uniform mat4 view;

layout(binding=0) uniform sampler2D texture0;
layout(binding=1) uniform sampler2D shadowMap;

const int maxNumberOfLights = 82;
uniform int numberOfLights;
uniform vec3 position[maxNumberOfLights];
uniform vec3 specular[maxNumberOfLights];
uniform vec3 diffuse[maxNumberOfLights];
uniform float constantAttenuation[maxNumberOfLights];
uniform float linearAttenuation[maxNumberOfLights];
uniform float quadraticAttenuation[maxNumberOfLights];
uniform float spotExponent[maxNumberOfLights];

in VertexData
{	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
	vec3 ShadowCoord;
} Input;

vec3 scene_ambient = vec3(0.5, 0.5, 0.5);

out vec4 fragmentColor;

void main()
{

	// Texture
	vec4 texel = texture2D(texture0, Input.TextureCoord);
	//vec4 texel = (blend.x * texel0) + (blend.y * texel1) + (blend.z * texel2);

	//
	// Phong shading
	//

	// Ambient light
	vec3 La = scene_ambient;	// Ambient light
	vec3 Ks = vec3(0.3, 0.3, 0.3);	// Specular reflectance
	vec3 Kd = vec3(1.0, 1.0, 1.0);	// Diffuse reflectance
	vec3 Ka = vec3(1.0, 1.0, 1.0);	// Ambient reflectance
	vec3 Is;
	vec3 Id;

	// Shadows
	//float cosTheta = clamp(dot(Input.Normal, vec3(0, 1, 0)), 0.0, 1.0);
	//float bias = 0.001 * tan(acos(cosTheta)); // cosTheta is dot( n,l ), clamped between 0 and 1
	//bias = clamp(bias, 0.0, 0.01);
	float visibility = 1.0;
	if (Input.ShadowCoord.x >= 0.0 && Input.ShadowCoord.x <= 1.0 && Input.ShadowCoord.y >= 0.0 && Input.ShadowCoord.y <= 1.0)
	{	float bias = 0.00005;
		vec4 shadowMapValue = texture(shadowMap, Input.ShadowCoord.xy);
		if (shadowMapValue.z < clamp(Input.ShadowCoord.z - bias, 0, 1))
		{	visibility = 0.3;
		}
	}

	vec3 totalLighting = La * Ka * visibility;

	float attenuation;

	for(int i = 0; i < numberOfLights && i < maxNumberOfLights; i++)
	{	// Light
		//vec3 lightPosition = vec3(0, 0, 2);
		vec3 Ls = specular[i];	// Specular light
		vec3 Ld = diffuse[i];	// Diffuse light

		vec3 lightPosView = vec3(view * vec4(position[i], 1.0));
		vec3 surfacePosition = vec3(model * vec4(Input.Position, 1.0));
		vec3 surfacePosView = vec3(view * vec4(surfacePosition, 1.0));
		vec3 surfaceToLight = normalize(lightPosView - surfacePosView);
		mat3 normalMatrix = transpose(inverse(mat3(view * model)));
		vec3 surfaceNormal = normalize(normalMatrix * Input.Normal);

		float dist = length(position[i] - surfacePosition);

		attenuation = 1.0 / (constantAttenuation[i]
		                     + linearAttenuation[i] * dist
		                     + quadraticAttenuation[i] * pow(dist, 2.0));
		//attenuation = attenuation * pow(clampedCosine, spotExponent[i]);

		// Diffuse light
		float dotProd = dot(surfaceToLight, surfaceNormal);
		dotProd = max(dotProd, 0.0);

		Id = Ld * Kd * abs(dotProd) * attenuation;

		// Specular light
		vec3 reflection = reflect(-surfaceToLight, surfaceNormal);
		float dotSpecular = dot(reflection, normalize(-surfacePosView));
		dotSpecular = max(dotSpecular, 0.0);
		float specularFactor = pow(dotSpecular, 30.0); // Specular factor

		Is = attenuation * Ls * Ks * specularFactor;

		totalLighting = totalLighting + Id + Is;
	}

	fragmentColor = vec4(totalLighting, 1.0) * texel;


	//fragmentColor = vec4(Id, 1.0) * texel;

	//fragmentColor = texel;
}