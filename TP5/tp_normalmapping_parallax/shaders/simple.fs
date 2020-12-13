#version 330 core

in vec3 p_world;
in vec3 n_world;
in vec2 uv_obj;
flat in mat3 TBN;

out vec4 color;

uniform sampler2D textureSampler;
uniform vec3 camera;
uniform sampler2D textureSampler1;
uniform sampler2D textureNormals;
uniform sampler2D textureAO;

void main()
{

    vec2 uv_obj_toUse = uv_obj;

    // //Uncomment to use parallax mapping
    // vec4 tex = texture(textureSampler1, uv_obj);
    // vec3 viewDir = normalize((TBN*camera) - (TBN*p_world));
    // vec2 p = (tex.r)*0.05 * viewDir.xy ;/// viewDir.z;
    // uv_obj_toUse = uv_obj + vec2(-p.x, p.y);

    // if (uv_obj_toUse.x < 0 || uv_obj_toUse.x > 1){
    //     discard;
    // }
    // if (uv_obj_toUse.y < 0 || uv_obj_toUse.y > 1){
    //     discard;
    // }


    vec3 n_toUse = vec3((texture(textureNormals, uv_obj_toUse) * 2.0) - 1.0);
    //Comment to work into the tangent space
    // n_toUse = n_world;    

    float Ka= 0.2;
    float Kd= 0.8;
    float Ks= 0.6;
    float Ia= Ka;
    
    //Comment to remove ambiant occlusion
    float AO = texture(textureAO, uv_obj).x; 
    // //Uncomment to use a threshold and increase even more the shadows
    // if (AO < 0.75)
    //     AO /= 2;
    // Ia = Ia*AO;
    
    vec3 Ul= normalize(TBN*vec3(0.0,2.0,0.0)-TBN*p_world);
    float Id= Kd * dot(n_toUse, Ul);
    vec3 t= normalize(TBN*camera - TBN*p_world);
    vec3 s= normalize(reflect(Ul,n_toUse));
    float es= 256;
    float Is= Ks * pow(dot(s,t),1);

    vec4 c = texture( textureSampler, uv_obj_toUse );
    color = (Ia + Id)*c;// + Is ;
};
