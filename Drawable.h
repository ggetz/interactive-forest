#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__
#include "Angel.h"
#include "DirectionalLight.h"
#include "Spotlight.h"
#include "Material.h"
#include "Camera.h"

using namespace std;

class Drawable 
{

protected:
	GLuint programs[1];
	GLuint VAOs[1];
	GLuint buffers[2];
	mat4 modelmatrix;
	static unsigned char* ppmRead(char* filename, int* width, int* height);

public:
	void setModelMatrix(mat4 mm) { modelmatrix = mm; }
	mat4 getModelMatrix() { return modelmatrix; }

	Material material;

};

#endif
