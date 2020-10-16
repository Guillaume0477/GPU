#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

out vec3 pos;
out vec3 n;
out vec2 uv;

void main()
{
	pos = position + vec3(0.0f, 0.1f, 0.0f);
	n = normal;
	uv = texcoord;
};
