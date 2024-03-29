#version 150

in vec4 vPosition;
in vec3 vNormal;
in vec2 vUV;

out vec4 vColor, shadowCoord;
out vec2 texCoord;
out float fogAmount;

uniform mat4 model_matrix, view_matrix, proj_matrix, shadow_matrix;

uniform vec4 eye;

uniform vec4 ambient, diffuse, spec;
uniform float shininess;

uniform vec3 light_direction;
uniform vec4 light_ambient, light_diffuse, light_specular;

uniform float fog_start, fog_end;

float fogFactorLinear(const float dist, const float start, const float end) 
{
  return 1.0 - clamp((end - dist) / (end - start), 0.0, 1.0);
}

void main() 
{
  texCoord = vUV;
  vec4 pos = model_matrix * vPosition;

  vec3 N = normalize(vNormal);
  vec3 L = normalize(light_direction);
  vec3 E = normalize(eye.xyz - pos.xyz);

  float Kd = max(dot(N, L), 0.0);
  float Ks = pow(max(dot(N, E), 0.0), shininess);

  vec4 d = Kd * diffuse * light_diffuse;
  vec4 s = Ks * spec * light_specular;

  vColor = (ambient * light_ambient) + d + s;
  vColor.a = 1.0;

  gl_Position = proj_matrix * view_matrix * pos;

  shadowCoord = shadow_matrix * vPosition;

  fogAmount = fogFactorLinear(length(pos - eye), fog_start, fog_end);
}