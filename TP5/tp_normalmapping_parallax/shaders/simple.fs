#version 330 core

in vec3 p_world;
in vec3 n_world;
in vec2 uv_obj;
in mat3 TBN;

out vec4 color;

uniform sampler2D textureSampler;
uniform vec3 camera;
uniform sampler2D textureSampler1;
uniform sampler2D textureNormals;

void main()
{

    
    vec3 n_toUse = vec3(texture(textureNormals, uv_obj)); // * 2.0) - 1.0);
    //Comment to work into the tangent space
    // n_toUse = n_world;    

    float Ka= 0.2;
    float Kd= 0.8;
    float Ks= 0.6;
    float Ia= Ka;
    vec3 Ul= normalize(vec3(0.0,2.0,0.0)-p_world);
    Ul = TBN * Ul;
    float Id= Kd * dot(n_toUse, Ul);
    vec3 t= normalize(camera - p_world);
    t = TBN * t;
    vec3 s= normalize(reflect(Ul,n_toUse));
    float es= 256;
    float Is= Ks * pow(dot(s,t),es);

    vec4 c = texture( textureSampler, uv_obj );
    color = (Ia + Id)*c + Is ;
};
