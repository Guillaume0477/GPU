#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

uniform mat4 MVP;

in vec3 v_pos[];

void main(void)
{
	gl_Position = MVP * vec4(v_pos[0], 1.);
	EmitVertex();
	gl_Position = MVP * vec4(v_pos[1], 1.);
	EmitVertex();
	gl_Position = MVP * vec4(v_pos[2], 1.);
	EmitVertex();
	gl_Position = MVP * vec4(v_pos[0], 1.);
	EmitVertex();
	EndPrimitive();
}
