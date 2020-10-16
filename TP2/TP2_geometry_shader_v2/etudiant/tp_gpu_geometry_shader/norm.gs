#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

uniform mat4 MVP;

in vec3 v_pos[];

void main(void)
{
    vec3 point = vec3(1.0/3*v_pos[0] + 1.0/3*v_pos[1] + 1.0/3*v_pos[2]);
    vec3 Tnorm = normalize(cross(v_pos[1]-v_pos[0], v_pos[2]-v_pos[0]));

    gl_Position = MVP * vec4(point, 1.0);
    EmitVertex();
    gl_Position = MVP * vec4(point+Tnorm*0.05, 1.0);
    EmitVertex();

	EndPrimitive();
}