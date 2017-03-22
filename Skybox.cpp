#include "Skybox.h"



Skybox::Skybox()
{
	_verts = vector<vec4>();
	_textureSize = 512;

	// Add the initial object positions
	_points = vector<vec4>();

	_points.push_back(vec4(-1.0, -1.0, 1.0, 1.0));
	_points.push_back(vec4(-1.0, 1.0, 1.0, 1.0));
	_points.push_back(vec4(1.0, 1.0, 1.0, 1.0));
	_points.push_back(vec4(1.0, -1.0, 1.0, 1.0));

	_points.push_back(vec4(-1.0, -1.0, -1.0, 1.0));
	_points.push_back(vec4(-1.0, 1.0, -1.0, 1.0));
	_points.push_back(vec4(1.0, 1.0, -1.0, 1.0));
	_points.push_back(vec4(1.0, -1.0, -1.0, 1.0));

	createFace(1, 0, 3, 2); // front
	createFace(2, 3, 7, 6); // right
	createFace(3, 0, 4, 7); // bottom
	createFace(6, 5, 1, 2); // top
	createFace(4, 5, 6, 7); // back
	createFace(5, 4, 0, 1); // left

	modelmatrix = mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
		);

	material = Material();
}


Skybox::~Skybox()
{
	glDeleteBuffers(1, buffers);
	glDeleteVertexArrays(1, VAOs);
}

void Skybox::createFace(int a, int b, int c, int d)
{
	// first triangle
	_verts.push_back(_points[a]);
	_verts.push_back(_points[b]);
	_verts.push_back(_points[c]);

	// second triangle
	_verts.push_back(_points[c]);
	_verts.push_back(_points[d]);
	_verts.push_back(_points[a]);
}

void Skybox::init()
{
	programs[0] = InitShader("vshader00_v150.glsl", "fshader00_v150.glsl");

	glGenBuffers(1, buffers);
	glGenVertexArrays(1, VAOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, _verts.size() * sizeof(vec4), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, _verts.size() * sizeof(vec4), &_verts.front());

	glBindVertexArray(VAOs[0]);
	glUseProgram(programs[0]);

	GLuint vPosition = glGetAttribLocation(programs[0], "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glGenTextures(1, &_texture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	skyTop = ppmRead("textures/sky-top.ppm", &_textureSize, &_textureSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, _textureSize, _textureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyTop);
	skyBottom = ppmRead("textures/sky-bottom.ppm", &_textureSize, &_textureSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, _textureSize, _textureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyBottom);
	skyRight = ppmRead("textures/sky-right.ppm", &_textureSize, &_textureSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, _textureSize, _textureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyRight);
	skyLeft = ppmRead("textures/sky-left.ppm", &_textureSize, &_textureSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, _textureSize, _textureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyLeft);
	skyFront = ppmRead("textures/sky-front.ppm", &_textureSize, &_textureSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, _textureSize, _textureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyFront);
	skyBack = ppmRead("textures/sky-back.ppm", &_textureSize, &_textureSize);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, _textureSize, _textureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, skyBack);
}

void Skybox::draw(mat4 viewMatrix, mat4 projMatrix)
{
	glBindVertexArray(VAOs[0]);

	glUseProgram(programs[0]);

	GLuint model_loc = glGetUniformLocation(programs[0], "model_matrix");
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, modelmatrix);

	GLuint view_loc = glGetUniformLocation(programs[0], "view_matrix");
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, viewMatrix);

	GLuint proj_loc = glGetUniformLocation(programs[0], "proj_matrix");
	glUniformMatrix4fv(proj_loc, 1, GL_TRUE, projMatrix);

	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
	glUniform1i(glGetUniformLocation(programs[0], "cubeMap"), 0);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDrawArrays(GL_TRIANGLES, 0, _verts.size());
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}