#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangant;
layout (location = 3) in vec2 texcoord;

uniform vec3 camera; // in world space
uniform mat4 MVP;
uniform mat4 model;
uniform sampler2D textureSampler1;


out vec3 p_world;
out vec3 n_world;
out vec2 uv_obj;
flat out mat3 TBN;

void main()
{
  uv_obj = texcoord;
 
 // On suppose aucun scaling
  p_world = (model * vec4(position, 1.0)).xyz;

  //Uncomment to use the height map
  vec4 tex = texture(textureSampler1, uv_obj);

  // Uncomment to modify the heigh of the object
  p_world.y = p_world.y + tex.x*0.7;

  vec3 t_world = mat3(model) * tangant.xyz;
  n_world = mat3(model) * normal;
  vec3 b = normalize(cross(t_world, n_world)*tangant.w);

  // Matrice de passage du repere monde au repere tangant
  TBN = transpose(mat3(t_world, b, n_world));

  //Comment to work into the tangent space
  // TBN = mat3(1.0);
  
  gl_Position = MVP*vec4(p_world, 1.0);
};
