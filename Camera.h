// Shreya Patel
// cs432
// hw6

#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Angel.h"
#include <vector>

class Camera {
public:
	Camera();
    
    // camera transformation functions
    void yaw(int);
    void pitch(int);
    void roll(int);
    void moveCamera(int);
    void positionCamera(vec4, vec4, vec4, vec4);
    mat4 getViewMatrix() const { return viewMatrix;}
    vec4 getEye() const { return eye; }
    mat4 getProjMatrix() const { return projectionMatrix; }
    void setMVMatrix();

public:
	vec4 eye, u, v, n;
	mat4 viewMatrix, projectionMatrix;
};

#endif
