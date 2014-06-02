#version 430

layout (binding=0) uniform sampler2D DiffuseTexture;
layout (binding=1) uniform sampler2D ShadowTexture;
layout (binding=2) uniform sampler2D NormalMapTexture;
layout (binding=3) uniform sampler2D SpecularMapTexture;

//TerrainTextures
layout (binding=4) uniform sampler2D TextureRed;
layout (binding=5) uniform sampler2D TextureRedNormal;
layout (binding=6) uniform sampler2D TextureRedSpecular;
layout (binding=7) uniform sampler2D TextureGreen;
layout (binding=8) uniform sampler2D TextureGreenNormal;
layout (binding=9) uniform sampler2D TextureGreenSpecular;
layout (binding=10) uniform sampler2D TextureBlue;
layout (binding=11) uniform sampler2D TextureBlueNormal;
layout (binding=12) uniform sampler2D TextureBlueSpecular;

uniform float TextureRepeats; //Determines how many times the textures will loop over the terrain
uniform vec3 SunDirection_cameraspace;
uniform mat4 V;

in VertexData
{
	vec3 Position;
	vec3 Normal;
	vec2 TextureCoord;
	vec4 ShadowCoord;
	vec3 Tangent;
	vec3 BiTangent;
} Input;

out vec4 frag_Diffuse;
out vec4 frag_Position;
out vec4 frag_Normal;
out vec4 frag_Specular;

float Shadow(vec4 ShadowCoord, vec3 normal)
{
	return 1.0;

	if (Input.ShadowCoord.x < 0.0 || Input.ShadowCoord.x > 1.0 || Input.ShadowCoord.y < 0.0 || Input.ShadowCoord.y > 1.0)
		return 0.9;

	//Variable bias
	vec3 n = normalize(normal);
	vec3 l = normalize(SunDirection_cameraspace);
	float cosTheta = clamp(dot(n, l), 0.0, 1.0);
	float bias = tan(acos(cosTheta));
	bias = clamp(bias, 0.0, 0.00003);

	//Fixed bias
	bias = 0;

	if( texture(ShadowTexture, Input.ShadowCoord.xy).z < ShadowCoord.z + bias)
	{
		return 0.6;
	}
	else 
	{
		return 1.0;
	}
}

void main()
{
	vec4 BlendMap = texture(DiffuseTexture, Input.TextureCoord);

	vec4 TextureRedTexel = texture(TextureRed, Input.TextureCoord * TextureRepeats);
	vec4 TextureRedTexelNormal = texture(TextureRedNormal, Input.TextureCoord * TextureRepeats);
	//vec4 TextureRedTexelSpecular = texture(TextureRedSpecular, Input.TextureCoord * TextureRepeats);

	vec4 TextureGreenTexel = texture(TextureGreen, Input.TextureCoord * TextureRepeats);
	vec4 TextureGreenTexelNormal = texture(TextureGreenNormal, Input.TextureCoord * TextureRepeats);
	//vec4 TextureGreenTexelSpecular = texture(TextureGreenSpecular, Input.TextureCoord * TextureRepeats);

	vec4 TextureBlueTexel = texture(TextureBlue, Input.TextureCoord * TextureRepeats);
	vec4 TextureBlueTexelNormal = texture(TextureBlueNormal, Input.TextureCoord * TextureRepeats);
	//vec4 TextureBlueTexelSpecular = texture(TextureBlueSpecular, Input.TextureCoord * TextureRepeats);

	//Mix the Terrain-textures together
	TextureRedTexel *= BlendMap.r;
	TextureGreenTexel = mix(TextureRedTexel, TextureGreenTexel, BlendMap.g);
	vec4 finalBlendTexel = mix(TextureGreenTexel, TextureBlueTexel, BlendMap.b);

	TextureRedTexelNormal *= BlendMap.r;
	TextureGreenTexelNormal = mix(TextureRedTexelNormal, TextureGreenTexelNormal, BlendMap.g);
	vec4 finalBlendTexelNormal = mix(TextureGreenTexelNormal, TextureBlueTexelNormal, BlendMap.b);

	//TextureRedTexelSpecular *= BlendMap.r;
	//TextureGreenTexelSpecular = mix(TextureRedTexelSpecular, TextureGreenTexelSpecular, BlendMap.g);
	//vec4 finalBlendTexelSpecular = mix(TextureGreenTexelSpecular, TextureBlueTexelSpecular, BlendMap.b);
	
	// G-buffer Position
	frag_Position = vec4(Input.Position.xyz, 1.0);

	// G-buffer Normal
	mat3 TBN = mat3(Input.Tangent, Input.BiTangent, Input.Normal);
	frag_Normal = normalize(vec4(TBN * vec3(finalBlendTexelNormal), 0.0));
	//frag_Diffuse = normalize(vec4(TBN * vec3(finalBlendTexelNormal), 0.0));
	//frag_Normal = vec4(Input.Normal, 0.0);

	// Diffuse Texture
	frag_Diffuse = finalBlendTexel;

	//G-buffer Specular
	frag_Specular = texture(SpecularMapTexture, Input.TextureCoord);
}