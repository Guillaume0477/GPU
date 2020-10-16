#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 3) out;

uniform mat4 MVP;

in vec3 v_pos[];

void main(void)
{
	vec3 n = normalize(cross(v_pos[1]-v_pos[0], v_pos[2]-v_pos[0]));	
	vec3 p = (v_pos[0] + v_pos[1] + v_pos[2]) / 3.;

	gl_Position = MVP * vec4(p, 1.0);
	EmitVertex();
	gl_Position = MVP * vec4(p + 0.04 * n, 1.0);
	EmitVertex();
	EndPrimitive();
}