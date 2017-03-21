#pragma once
#include "Angel.h"
#include <vector>

class Branch
{

public:
	Branch();
	Branch(Branch* head);
	Branch(Branch* head, Branch* parent);
	~Branch();

	Branch* child1;
	Branch* child2;
	Branch* parent;

	vec3 head;
	vec3 tangent;
	float length;
	float radius;

	vector<int> root;
};

