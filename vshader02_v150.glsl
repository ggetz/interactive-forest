#version 150

in vec4 vPosition;
uniform mat4 MVP_matrix;

void main() 
{
	gl_Position = MVP_matrix * vPosition;
}