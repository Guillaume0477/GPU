#version 330 core

in vec3 posi;

uniform sampler2D myTexture;

out vec4 color;

void main()
{


    float taille_pixel_x = 1.0/480;
    float taille_pixel_y = 1.0/360;
    vec2 pixel= (vec2(posi.x ,-posi.y)+1)/2;



	vec4 color1 = texture(myTexture, vec2((pixel.x - taille_pixel_x), (pixel.y - taille_pixel_y)));
    vec4 color2 = texture(myTexture, vec2((pixel.x - taille_pixel_x), (pixel.y + taille_pixel_y)));
    vec4 color3 = texture(myTexture, vec2((pixel.x - taille_pixel_x), (pixel.y )));
    vec4 color4 = texture(myTexture, vec2((pixel.x + taille_pixel_x), (pixel.y - taille_pixel_y)));
    vec4 color5 = texture(myTexture, vec2((pixel.x + taille_pixel_x), (pixel.y + taille_pixel_y)));
    vec4 color6 = texture(myTexture, vec2((pixel.x + taille_pixel_x), (pixel.y )));
    vec4 color7 = texture(myTexture, vec2((pixel.x ), (pixel.y - taille_pixel_y)));
    vec4 color8 = texture(myTexture, vec2((pixel.x ), (pixel.y + taille_pixel_y)));
    vec4 color9 = texture(myTexture, vec2((pixel.x ), (pixel.y )));

    vec4 color_moy= (color1 + color2 + color3 + color4 + color5 + color6 + color7 + color8 + color9)/9;



    //float color_grey =  (color1.x + color1.y + color1.z)/3;
    //vec3 color_grey_3 = vec3(color_grey,color_grey,color_grey);
    color = color_moy;
};
