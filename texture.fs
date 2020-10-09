#version 330 core

in vec3 posi;

uniform sampler2D myTexture;

out vec4 color;

void main()
{
	color = texture(myTexture, (vec2(posi.x, -posi.y)+1)/2);
};
