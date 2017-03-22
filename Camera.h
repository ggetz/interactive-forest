#pragma once
#include "Angel.h"
#include <vector>


using namespace std;

class Camera {
public:
	Camera();
    
    // camera transformation functions
    void yaw(int);
    void pitch(int);
    void roll(int);
    void moveCamera(int);
    void positionCamera(vec4, vec4, vec4, vec4);
    void changeAspect(int width, int height);
    mat4 getViewMatrix() const { return viewMatrix;}
    vec4 getEye() const { return eye; }
    mat4 getProjMatrix() const { return projectionMatrix; }
    void setMVMatrix();
    vec4 getPickingLocation(vec2 pFront);

public:
	vec4 eye, u, v, n;
	mat4 viewMatrix, projectionMatrix;
};

