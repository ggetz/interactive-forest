#pragma once
#include "Mesh.h"
#include <vector>

class Ground : public Mesh
{
    
public:
    Ground();

private:
    void build();
    void createFace(int a, int b, int c, int d);
    void calcNormals();
};


