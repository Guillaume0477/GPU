#version 330 core

out vec4 color;
uniform float time;

void main()
{
	color = vec4(0.8f*cos(time), 0.3f*sin(time), 0.3f, 0.6f);
};
