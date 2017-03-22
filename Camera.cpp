#include "Camera.h"
#include "mat.h"


Camera::Camera() 
{
    _eye = vec4(0, 2, 0, 1);
    _n = vec4(0, 0, 1, 0);
    _v = vec4(0, 1, 0, 0);
    _u = vec4(1, 0, 0, 0);
	_fov = 65;
	_near = 0.1;
	_far = 100.0;
	_width = _height = 1.0;
}

vec4 Camera::getPickingLocation(vec2 pFront)
{
    // convert pFront to pCam
    float t = _near * tan(_fov / 2);
    float r = t * (512/512);
    
    vec4 pCam = vec4(r*pFront[0], t*pFront[1], -_near, 1);
    
    // convert from pCam to pWorld
    mat4 inverseView = mat4(vec4(_u.x, _v.x, _n.x, _eye.x),
                            vec4(_u.y, _v.y, _n.y, _eye.y),
                            vec4(_u.z, _v.z, _n.z, _eye.z),
                            vec4(0,0,0,1));
    vec4 pWorld = inverseView * pCam;
    
    return pWorld;
}

void Camera::positionCamera(vec4 eye, vec4 n, vec4 v, vec4 u) 
{
	_eye = eye;
	_n = n;
	_v = v;
	_u = u;
}

// aspect ratio
void Camera::changeAspect(int width, int height) 
{
	_width = width;
	_height = height;
}

// camera transformations
void Camera::yaw(float deg)
{
    float radians = M_PI / 180 * deg; // convert to radians
	_n = cos(radians) * _n - sin(radians) * _u;
	_u = sin(radians) * _n + cos(radians) * _u;
}

void Camera::pitch(float deg)
{
    float radians = M_PI / 180 * deg; // convert to radians
	_v = cos(radians) * _v - sin(radians) * _n;
	_n = sin(radians) * _v + cos(radians) * _n;
}

void Camera::roll (float deg)
{
    float radians = M_PI / 180 * deg; // convert to radians
	_u = cos(radians) * _u - sin(radians) * _v;
	_v = sin(radians) * _u + cos(radians) * _v;
}

void Camera::moveForward (float change)
{
	_eye -= _n * change;
}

void Camera::moveRight (float change)
{
	_eye += _u * change;
}

void Camera::moveUp (float change)
{
	_eye += _v * change;
}

mat4 Camera::getViewMatrix()
{
	return LookAt(_eye, _eye - _n, _v);
}

mat4 Camera::getProjMatrix()
{
	if (_perspective)
	{
		return Perspective(_fov, 1.0, _near, _far);
	}

	return Ortho(-1.0, 1.0, -1.0, 1.0, 1.0, 3.0);
}
