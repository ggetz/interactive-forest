#pragma once
#include "Angel.h"
class Light
{
public:

	Light()
	{
		position = vec4(0.0, 0.0, 2.0, 0.0);
		ambient = vec4(0.8, 0.8, 0.8, 1.0);
		diffuse = vec4(1.0, 1.0, 1.0, 1.0);
		specular = vec4(1.0, 1.0, 1.0, 1.0);

		shadow = vec4(0.9, 0.9, 0.9, 1.0);

		fogColor = vec4(0.6, 0.6, 0.6, 1.0);
		fogStart = 6;
		fogEnd = 25;
	}

	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	vec4 shadow;

	vec4 fogColor;
	float fogStart;
	float fogEnd;
};

