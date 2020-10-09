#version 330 core

in vec3 posi;

uniform sampler2D myTexture;

out vec4 color;

void main()
{

	 vec4 color1 = texture(myTexture, (vec2(posi.x, -posi.y)+1)/2);
     float color_grey =  (color1.x + color1.y + color1.z)/3;
     vec3 color_grey_3 = vec3(color_grey,color_grey,color_grey);
     color = vec4(color_grey_3,1);
};


