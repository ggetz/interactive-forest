#include "SnowParticles.h"

SnowParticles::SnowParticles()
{
	
}

void SnowParticles::initializeParticles()
{
	_program = InitShader("vshader03_v150.glsl", "fshader03_v150.glsl");
	glUseProgram(_program);

	for (int i = 0; i < NUM_PARTICLES; ++i) {
		particles[i].mass = 1.0;
		for (int j = 0; j < 3; j++) {
			particles[i].color[j] = 1;
			particles[i].position[j] = 2.0*((float)rand() / RAND_MAX) - 1.0;
			particles[i].velocity[j] = 0; //2.0*((float)rand() / RAND_MAX) - 1.0;
		}
		particles[i].color.w = 1.0;
		particles[i].position.w = 1.0;
		particles[i].velocity.w = 1.0;
	}

	_modelmatrix = mat4();
}

void SnowParticles::updateParticles(float dt)
{
	for (int i = 0; i < NUM_PARTICLES; ++i) {
		for (int j = 0; j < 3; j++) {
			particles[i].position[j] += dt * particles[i].velocity[j];
			//particles[i].velocity[j] += dt * applyForces(i,j)/particles[i].mass;
		}
	}
}

float SnowParticles::applyForces(int i, int j) {
	if (j == 1) // only affect y direction
		return -1;
	else
		return 0;
}

void SnowParticles::drawParticals(Camera camera) {
	glBindVertexArray(_vao);
	glUseProgram(_program);

	for (int i = 0; i < NUM_PARTICLES; ++i) {
		particlePoints[i] = particles[i].position;
		particleColors[i] = particles[i].color;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _buffer);
	glBufferData(GL_ARRAY_BUFFER, particlePoints.size() * sizeof(vec4) + particleColors.size() * sizeof(vec4), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particlePoints.size() * sizeof(vec4), &particlePoints.front());
	glBufferSubData(GL_ARRAY_BUFFER, particlePoints.size() * sizeof(vec4), particleColors.size() * sizeof(vec4), &particleColors.front());

	GLuint model_loc = glGetUniformLocation(_program, "model_matrix");
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, _modelmatrix);
	
	GLuint view_loc = glGetUniformLocation(_program, "view_matrix");
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, camera.getViewMatrix());

	GLuint projection_loc = glGetUniformLocation(_program, "proj_matrix");
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, camera.getProjMatrix());

	glPointSize(3.0);
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
}



SnowParticles::~SnowParticles()
{
}
