#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:

	DirectionalLight()
	{
		direction = vec3(-1.0, 0.0, 0.0);
	}

	vec3 direction;
};

