#pragma once
#include "Angel.h"
#include "TreeProperties.h"
#include <vector>

using namespace std;

class Branch
{

public:
	Branch();
	Branch(vec3 head);
	Branch(vec3 head, Branch* parent);
	~Branch();

	Branch* child1;
	Branch* child2;
	Branch* parent;

	vec3 head;
	vec3 tangent;
	float length;
	float radius;
	bool trunk;

	vector<int> root;
	vector<int> ring0;
	vector<int> ring1;
	vector<int> ring2;
	int end;

	void split(int level, int steps, TreeProperties properties, int l1, int l2);
	vec3 mirrorBranch(vec3 vec, vec3 norm, TreeProperties properties);
};