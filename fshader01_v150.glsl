#version 150

in vec4 vColor, shadowCoord;
in vec2 texCoord;

uniform sampler2D textureID;
uniform sampler2D shadow_map;
uniform vec4 shadow_color;

out vec4 fragColor;

void main() 
{ 
	vec3 shadowMapTexCoord = shadowCoord.xyz / shadowCoord.w;
	shadowMapTexCoord = 0.5 * shadowMapTexCoord + 0.5;

	vec2 poissonDisk[4] = vec2[](
	  vec2( -0.94201624, -0.39906216 ),
	  vec2( 0.94558609, -0.76890725 ),
	  vec2( -0.094184101, -0.92938870 ),
	  vec2( 0.34495938, 0.29387760 )
	);

	float shade = 0.0;

	if (shadowMapTexCoord.z >= 0 && 
		shadowMapTexCoord.z <= 1 && 
		shadowMapTexCoord.x >= 0 &&
		shadowMapTexCoord.x <= 1 && 
		shadowMapTexCoord.y >= 0 && 
		shadowMapTexCoord.y <= 1)
	{
		for (int i = 0; i < 4; i++){
			if (texture(shadow_map, shadowMapTexCoord.st + poissonDisk[i]/600.0).r < shadowMapTexCoord.z - 0.01)
			{
				shade += 0.25;
			}
		}
	}

    fragColor = texture(textureID, texCoord) * vColor * (1 - (1 - shadow_color) * shade);
} 