#pragma once
#include "Drawable.h"
#include <vector>

class Skybox : public Drawable
{

public:
	Skybox();
	~Skybox();

	void init();
	void draw(mat4 viewMatrix, mat4 projMatrix);

private:
	vector<vec4> _points;
	vector<vec4> _verts;
	int _textureSize;
	GLuint _texture;
	GLubyte* skyTop;
	GLubyte* skyBottom;
	GLubyte* skyRight;
	GLubyte* skyLeft;
	GLubyte* skyFront;
	GLubyte* skyBack;

	void createFace(int a, int b, int c, int d);
};

