#pragma once
#include "Angel.h"
#include "TreeProperties.h"
#include "Branch.h"
#include "Mesh.h"
#include <vector>
#include <iostream>

using namespace std;

class Tree
{

public:
	Tree();
	Tree(TreeProperties properties);
	~Tree();

	Mesh* createTreeMesh()
	{
		return new Mesh(_verts, _faces, _normals, _uvs);
	}

	Mesh* createTwigMesh()
	{
		return new Mesh(_vertsTwigs, _facesTwigs, _normalsTwigs, _uvsTwigs);
	}

private:
	TreeProperties _properties;

	Branch* _root;

	vector<vec4> _verts;
	vector<unsigned int> _faces;
	vector<vec3> _normals;
	vector<vec2> _uvs;

	vector<vec4> _vertsTwigs;
	vector<unsigned int> _facesTwigs;
	vector<vec3> _normalsTwigs;
	vector<vec2> _uvsTwigs;

	void build();
	void createForks(Branch* branch, float radius);
	void createTwigs(Branch* branch);
	void doFaces(Branch* branch);
	void calcNormals();
};

