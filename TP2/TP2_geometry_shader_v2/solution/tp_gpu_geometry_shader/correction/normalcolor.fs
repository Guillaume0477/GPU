#version 330 core

out vec4 color;

in vec3 g_n;

void main()
{
	color = vec4(0.5*g_n+0.5, 1.f);
};
