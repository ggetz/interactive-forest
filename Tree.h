#pragma once
#include "Angel.h"
#include "TreeProperties.h"
#include "Branch.h"
#include <vector>

using namespace std;

class Tree
{

public:
	Tree();
	Tree(TreeProperties properties);
	~Tree();

private:
	TreeProperties _properties;

	Branch* _root;

	vector<vec4> _verts;
	vector<int> _faces;
	vector<vec3> _normals;
	vector<vec2> _uvs;

	vector<vec4> _vertsTwigs;
	vector<int> _facesTwigs;
	vector<vec3> _normalsTwigs;
	vector<vec2> _uvsTwigs;

	void createForks(Branch* branch, float radius);
	void createTwigs(Branch* branch);
	void doFaces(Branch* branch);
	void calcNormals();
};

