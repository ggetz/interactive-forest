#include "Camera.h"
#include "mat.h"

GLfloat fov = 65;
GLfloat nearp = 1.0;
GLfloat farp = 100.0;

Camera::Camera() {
    eye = vec4(0, 2, 0, 1);
    n = vec4(0, 0, 1, 0);
    v = vec4(0, 1, 0, 0);
    u = vec4(1, 0, 0, 0);
    setMVMatrix();
}

vec4 Camera::getPickingLocation(vec2 pFront)
{
    // convert pFront to pCam
    float t = nearp * tan(fov / 2);
    float r = t * (512/512);
    
    vec4 pCam = vec4(r*pFront[0], t*pFront[1], -nearp, 1);
    
    // convert from pCam to pWorld
    mat4 inverseView = mat4(vec4(u.x, v.x, n.x, eye.x),
                            vec4(u.y, v.y, n.y, eye.y),
                            vec4(u.z, v.z, n.z, eye.z),
                            vec4(0,0,0,1));
    vec4 pWorld = inverseView * pCam;
    
    return pWorld;
}

void Camera::positionCamera(vec4 ei, vec4 ni, vec4 vi, vec4 ui) {
    eye = ei;
    n = ni;
    u = ui;
    v = vi;
    //setMVMatrix();
    viewMatrix = LookAt(eye, eye - n, v);
}

void Camera::setMVMatrix()
{
    vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
    mat4 m = mat4(u,v,n,t) * Translate(-eye);

    viewMatrix = m;
}


// aspect ratio
void Camera::changeAspect(int width, int height) 
{
    projectionMatrix = Perspective(fov, GLfloat(width) / height, nearp, farp);
}

// camera transformations
void Camera::yaw(int deg)
{
    float radians = M_PI/180 * deg; // convert to radians
    float c = cosf(radians);
    float s = sinf(radians);
    
    vec4 u1 = u;
    vec4 n1 = n;
    
    u = vec4(c*u1.x - s*n1.x, c*u1.y - s*n1.y, c*u1.z - s*n1.z, 0.0);
    n = vec4(s*u1.x + c*n1.x, s*u1.y + c*n1.y, s*u1.z + c*n1.z, 0.0);
    setMVMatrix();
}

void Camera::pitch(int deg)
{
    float radians = M_PI/180 * deg; // convert to radians
    float c = cosf(radians);
    float s = sinf(radians);
    
    vec4 v1 = v;
    vec4 n1 = n;
    
    v = vec4(c*v1.x - s*n1.x, c*v1.y - s*n1.y, c*v1.z - s*n1.z, 0.0);
    n = vec4(s*v1.x + c*n1.x, s*v1.y + c*n1.y, s*v1.z + c*n1.z, 0.0);
    setMVMatrix();
}

void Camera::roll(int deg)
{
    float radians = M_PI/180 * deg; // convert to radians
    float c = cosf(radians);
    float s = sinf(radians);
    
    vec4 u1 = u;
    vec4 v1 = v;
    
    u = vec4(c*u1.x + s*v1.x, c*u1.y + s*v1.y, c*u1.z + s*v1.z, 0.0);
    v = vec4(-s*u1.x + c*v1.x, -s*u1.y + c*v1.y, -s*u1.z + c*v1.z, 0.0);
    setMVMatrix();
}

void Camera::moveCamera(int change)
{
    vec4 e1 = eye;
    vec4 newEye;
    newEye = vec4(e1.x, e1.y+ change, e1.z , 1);
    eye = newEye;
    viewMatrix = LookAt(eye, eye - n, v);
}
