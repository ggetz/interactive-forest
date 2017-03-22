#pragma once
#include "Angel.h"
#include "material.h"
#include "camera.h"
#include "DirectionalLight.h"
#include <vector>
#include <iostream>

using namespace std;

class Mesh
{

public:
	Mesh();
	Mesh(vector<vec4> verts, vector<unsigned int> faces, vector<vec3> normals, vector<vec2> uvs);
	~Mesh();

	void init();
	void draw(Camera camera, DirectionalLight light, GLuint shadowMap);
	void drawShadowMap(DirectionalLight light);
	void setMaterial(Material material) { _material = material; }
	void setVerts(vector<vec4> verts) { _verts = verts; }
	void setNormals(vector<vec3> normals) { _normals = normals; }
	void setUVs(vector<vec2> uvs) { _uvs = uvs; }
	void setFaces(vector<unsigned int> faces) { _faces = faces; }
	void setPosition(vec4 position) 
	{
		_modelmatrix[0][3] = position.x;
		_modelmatrix[1][3] = position.y;
		_modelmatrix[2][3] = position.z;
	}

	static unsigned char* ppmRead(char* filename, int* width, int* height);

protected:
	vector<vec4> _verts;
	vector<unsigned int> _faces;
	vector<vec3> _normals;
	vector<vec2> _uvs;
	GLuint _program;
	GLuint _shadowProgram;
	GLuint _vao;
	GLuint _buffers[2];
	GLuint _texture;
	mat4 _modelmatrix;
	Material _material;
};

