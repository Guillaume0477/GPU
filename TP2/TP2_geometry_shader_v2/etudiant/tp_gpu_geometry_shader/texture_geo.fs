#version 330 core

in vec2 g_uv;

out vec4 color;

uniform sampler2D myTexture;

void main()
{
	color = texture(myTexture, g_uv);
};



