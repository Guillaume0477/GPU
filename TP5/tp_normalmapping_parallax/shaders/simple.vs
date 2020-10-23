#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangant;
layout (location = 3) in vec2 texcoord;

uniform vec3 camera; // in world space
uniform mat4 MVP;
uniform mat4 model;

out vec3 p_world;
out vec3 n_world;
out vec2 uv_obj;

void main()
{
  uv_obj = texcoord;
 
 // On suppose aucun scaling
  p_world = (model * vec4(position, 1.0)).xyz;
  vec3 t_world = mat3(model) * tangant.xyz;
  n_world = mat3(model) * normal;
  vec3 b = normalize(cross(t_world, n_world)*tangant.w);
  // Matrice de passage du repere monde au repere tangant
  mat3 TBN = transpose(mat3(t_world, b, n_world));

  gl_Position = MVP*vec4(position, 1.0);
};
