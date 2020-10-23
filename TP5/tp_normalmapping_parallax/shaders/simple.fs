#version 330 core

in vec3 p_world;
in vec3 n_world;
in vec2 uv_obj;

out vec4 color;

uniform sampler2D textureSampler;

void main()
{
    color = texture( textureSampler, uv_obj );
};
