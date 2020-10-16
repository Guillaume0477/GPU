#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;


out vec3 v_pos;
out vec3 v_norm;
out vec2 uv;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(position, 1.0);
    v_pos = position;
    v_norm = normal;

    uv = texcoord;
};
