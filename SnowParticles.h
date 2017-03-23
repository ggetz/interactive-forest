#pragma once
#include "Angel.h"
#include "Camera.h"
#include <vector>

using namespace std;

class SnowParticles
{

	struct particle {
		vec4 color;
		vec4 position;
		vec4 velocity;
		float mass;
	};

public:
	SnowParticles();
	~SnowParticles();

	const int NUM_PARTICLES = 1000;
	vector<particle> particles = vector<particle>(NUM_PARTICLES);
	vector<vec4> particlePoints = vector<vec4>(NUM_PARTICLES);
	vector<vec4> particleColors = vector<vec4>(NUM_PARTICLES);

	void initializeParticles();
	void drawParticals(Camera);
	float applyForces(int, int);
	void updateParticles(float);
	void setPosition(vec4 position)
	{
		_modelmatrix[0][3] = position.x;
		_modelmatrix[1][3] = position.y;
		_modelmatrix[2][3] = position.z;
	}
	void setYRotation(float amount)
	{
		_modelmatrix[0][0] = cos(amount);
		_modelmatrix[2][0] = -sin(amount);
		_modelmatrix[0][2] = sin(amount);
		_modelmatrix[2][2] = cos(amount);
	}

private:
	GLuint _program;
	GLuint _vao;
	GLuint _buffer;
	mat4 _modelmatrix;
};

