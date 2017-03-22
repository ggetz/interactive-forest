#pragma once
#include "Angel.h"
#include <vector>


using namespace std;

class Camera {
public:
	Camera();
    
    // camera transformation functions
    void yaw(float);
    void pitch(float);
    void roll(float);
    void moveForward(float);
	void moveRight(float);
	void moveUp(float);
    void positionCamera(vec4, vec4, vec4, vec4);
    void changeAspect(int width, int height);
	vec4 getEye() const { return _eye; }
	void setEye(vec4 value) { _eye = value; }
	mat4 getViewMatrix();
	mat4 getProjMatrix();
    vec4 getPickingLocation(vec2 pFront);

private:
	vec4 _eye, _u, _v, _n;
	float _fov, _near, _far, _width, _height;
	bool _perspective;
};

