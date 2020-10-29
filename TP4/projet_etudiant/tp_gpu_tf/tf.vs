#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec3 vitesse;

uniform float time;

out vec3 pos;
out vec3 vit;


void main()
{
	pos = position + vitesse*time/1000;//*time;
    vit = vitesse ; //vec3(0.0f, -0.05f, 0.0f);//*time;

};
