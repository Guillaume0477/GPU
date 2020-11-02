#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec3 vitesse;

uniform float dt;

out vec3 pos;
out vec3 vit;



float rand(float pos){
    return fract(sin(pos*12.9898) * 43758.5453)-0.5;
}


void main()
{
    bool test;
    float mass = 0.01;


    pos = position + vitesse*dt;//*time;
    vit = vitesse + vec3(0.0f, -mass*9.81*dt, 0.0f);//*time;

    test = (pos.y > -2) ; //&& (pos[3*i+1] < 2);
    // test = test && (pos[3*i] > -2) && (pos[3*i] < 2);
    // test = test && (pos[3*i+2] > -2) && (pos[3*i+2] < 2);

    if (test == false){
        
        vit.x = rand(pos.x);
        vit.y = (rand(pos.y)+1);
        vit.z = rand(pos.z);

        pos.x = 0.0;
        pos.y = 0.0;
        pos.z = 0.0;

    }



};
