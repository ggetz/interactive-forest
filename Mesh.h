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
	Mesh(vec4 origin, vector<vec4> verts, vector<unsigned int> faces, vector<vec3> normals, vector<vec2> uvs);
	~Mesh();

	void init();
	void draw(Camera camera, DirectionalLight light);
	void setMaterial(Material material) { _material = material; }
	void setVerts(vector<vec4> verts) { _verts = verts; }
	void setNormals(vector<vec3> normals) { _normals = normals; }
	void setUVs(vector<vec2> uvs) { _uvs = uvs; }
	void setFaces(vector<unsigned int> faces) { _faces = faces; }

	static unsigned char* ppmRead(char* filename, int* width, int* height);

protected:
	vec4 _origin;
	vector<vec4> _verts;
	vector<unsigned int> _faces;
	vector<vec3> _normals;
	vector<vec2> _uvs;
	GLuint _program;
	GLuint _vao;
	GLuint _buffers[2];
	GLuint _texture;
	mat4 _modelmatrix;
	Material _material;
};

