#version 150

in vec4 vPosition;
in vec4 colorIn;
in vec4 vVelocity;

uniform float time;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

out vec4 color;

void main() 
{ 
  vec3 object_pos;
  object_pos.xyz = vPosition.xyz + vVelocity.xyz*time;

  gl_Position = proj_matrix * view_matrix * model_matrix * vec4(object_pos, 1);
  color = colorIn;
}