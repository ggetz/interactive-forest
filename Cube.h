#pragma once
#include "Mesh.h"
#include <vector>

class Cube : public Mesh
{

public:
	Cube();

private:
	void build();
	void createFace(int a, int b, int c, int d);
	void calcNormals();
};

