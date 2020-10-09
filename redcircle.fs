#version 330 core

in vec3 posi;
in vec3 cols;


uniform float radius;

out vec4 color;

void main()
{
    float eps = 0.01;
    float d = sqrt(posi.x*posi.x + posi.y*posi.y);
    if (abs(d - radius) < eps || abs(d-(3*radius/4)) < eps || abs(d-(radius/2)) < eps|| abs(d-(radius/4)) < eps){
        color = vec4(1,0,0,1);
    } else {
        color = vec4(cols,1);
    }
}


