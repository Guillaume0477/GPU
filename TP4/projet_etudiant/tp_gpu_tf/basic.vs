#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec3 vitesse;

uniform mat4 MVP;
out vec3 vit;

void main()
{
    gl_Position = MVP * vec4(position, 1.0);
    vit = vitesse;
};
