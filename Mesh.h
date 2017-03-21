#pragma once
#include "Angel.h"
#include "material.h"
#include "camera.h"
#include "DirectionalLight.h"
#include <vector>

using namespace std;

class Mesh
{

public:
	Mesh();
	Mesh(vec4 origin, vector<vec4> verts, vector<int> faces, vector<vec3> normals, vector<vec2> uvs);
	~Mesh();

	void init();
	void draw(Camera camera, DirectionalLight light);

	static unsigned char* ppmRead(char* filename, int* width, int* height);

protected:
	vec4 _origin;
	vector<vec4> _verts;
	vector<int> _faces;
	vector<vec3> _normals;
	vector<vec2> _uvs;
	GLuint _program;
	GLuint _vao;
	GLuint _buffers[2];
	GLuint _texture;
	mat4 _modelmatrix;
	Material _material;
};

