#pragma once
#include "Angel.h"

class Material
{
public:

	Material()
	{
		ambient = vec4(0.0, 0.0, 0.0, 1.0);
		diffuse = vec4(0.5, 0.5, 0.5, 1.0);
		specular = vec4(1.0, 1.0, 1.0, 1.0);
		shininess = 6.0;
	}

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;

	char* texturePath;
	int textureSize;
};

