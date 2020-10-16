#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 MVP;

in vec3 v_pos[];
in vec3 v_norm[];
in vec2 uv[];

out vec2 g_uv;

void main(void)
{
	for (int i = 0; i < 3; i++)
	{
    vec3 p = v_pos[i] + 0.05 * v_norm[i];
	  gl_Position = MVP * vec4(p, 1.0);
    g_uv = uv[i];
	  EmitVertex();
	}
	EndPrimitive();
}
