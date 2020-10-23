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

void main(){

    uint i = gl_GlobalInvocationID.x;
    pos[3*i] = pos[3*i] + 0.01*vit[3*i];
    pos[3*i+1] = pos[3*i+1] + 0.01*(vit[3*i+1]-0.1*9.81);
    pos[3*i+2] = pos[3*i+2] + 0.01*vit[3*i+2];

}