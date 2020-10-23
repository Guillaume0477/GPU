#version 430 core

layout(local_size_x = 10, local_size_y = 1, local_size_z = 1) in;


layout(std430, binding = 1) buffer positionBlock
{
    float pos[];
};

layout(std430, binding = 2) buffer vitesseBlock
{
    float vit[];
};

uniform float dt;

float rand(float pos){
    return fract(sin(pos*12.9898) * 43758.5453)-0.5;
}

void main(){
    bool test;
    float mass = 0.01;
    uint i = gl_GlobalInvocationID.x;
    pos[3*i] = pos[3*i] + dt*vit[3*i];
    pos[3*i+1] = pos[3*i+1] + dt*(vit[3*i+1]-mass*9.81*dt);
    pos[3*i+2] = pos[3*i+2] + dt*vit[3*i+2];

    vit[3*i+1] = vit[3*i+1]-mass*9.81*dt;
    
    test = (pos[3*i+1] > -2) ; //&& (pos[3*i+1] < 2);
    // test = test && (pos[3*i] > -2) && (pos[3*i] < 2);
    // test = test && (pos[3*i+2] > -2) && (pos[3*i+2] < 2);

    if (test == false){
        
        vit[3*i] = rand(pos[3*i]);
        vit[3*i+1] = (rand(pos[3*i+1])+1);
        vit[3*i+2] = rand(pos[3*i+2]);

        pos[3*i] = 0.0;
        pos[3*i+1] = 0.0;
        pos[3*i+2] = 0.0;


    }

}