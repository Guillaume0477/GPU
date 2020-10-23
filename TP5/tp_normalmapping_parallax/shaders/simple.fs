#version 330 core

in vec3 p_world;
in vec3 n_world;
in vec2 uv_obj;

out vec4 color;

uniform sampler2D textureSampler;
uniform vec3 camera;


void main()
{

    float Ka= 0.2;
    float Kd= 0.8;
    float Ks= 0.6;
    float Ia=Ka;
    vec3 Ul = normalize(vec3(0.0,2.0,0.0)-p_world);
    float Id=Kd * dot(n_world, Ul);
    vec3 t = normalize(camera - p_world);
    vec3 s = normalize(reflect(Ul,n_world));
    float es= 256;
    float Is= Ks * pow(dot (s,t),es);

    vec4 c = texture( textureSampler, uv_obj );
    color = (Ia + Id)*c + Is ;
};
