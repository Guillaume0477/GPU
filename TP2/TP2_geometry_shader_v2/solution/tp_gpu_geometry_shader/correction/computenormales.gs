#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 MVP;

in vec3 v_pos[];
out vec3 g_n;

void main(void)
{
	vec3 n = normalize(cross(v_pos[1]-v_pos[0], v_pos[2]-v_pos[0]));
	g_n = n;

	for (int i = 0; i < 3; i++)
	{
	  gl_Position = MVP * vec4(v_pos[i], 1.0);
	  EmitVertex();
	}
	EndPrimitive();
}
