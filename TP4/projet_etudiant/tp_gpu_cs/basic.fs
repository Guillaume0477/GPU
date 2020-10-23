#version 330 core

in vec3 vit;

out vec4 color;

vec4 colormapSummer(float x) {
    return vec4(clamp(x, 0.0, 1.0), clamp(0.5 * x + 0.5, 0.0, 1.0), 0.4, 1.0);
}

vec4 colormapSpring(float x) {
    return vec4(1.0, clamp(x, 0.0, 1.0), clamp(1.0 - x, 0.0, 1.0), 1.0);
}

float colormap_red(float x) {
    if (x < 0.7) {
        return 4.0 * x - 1.5;
    } else {
        return -4.0 * x + 4.5;
    }
}

float colormap_green(float x) {
    if (x < 0.5) {
        return 4.0 * x - 0.5;
    } else {
        return -4.0 * x + 3.5;
    }
}

float colormap_blue(float x) {
    if (x < 0.3) {
       return 4.0 * x + 0.5;
    } else {
       return -4.0 * x + 2.5;
    }
}

vec4 colormapJet(float x) {
    float r = clamp(colormap_red(x), 0.0, 1.0);
    float g = clamp(colormap_green(x), 0.0, 1.0);
    float b = clamp(colormap_blue(x), 0.0, 1.0);
    return vec4(r, g, b, 1.0);
}

vec4 colormapHot(float x) {
    float r = clamp(8.0 / 3.0 * x, 0.0, 1.0);
    float g = clamp(8.0 / 3.0 * x - 1.0, 0.0, 1.0);
    float b = clamp(4.0 * x - 3.0, 0.0, 1.0);
    return vec4(r, g, b, 1.0);
}

float colormap_red1(float x) {
	return ((((1.30858855846896E+03 * x - 2.84649723684787E+03) * x + 1.76048857883363E+03) * x - 3.99775093706324E+02) * x + 2.69759225316811E+01) * x + 2.54587325383574E+02;
}

float colormap_green1(float x) {
	return ((((-8.85605750526301E+02 * x + 2.20590941129997E+03) * x - 1.50123293069936E+03) * x + 2.38490009587258E+01) * x - 6.03460495073813E+01) * x + 2.54768707485247E+02;
}

float colormap_blue1(float x) {
	if (x < 0.2363454401493073) {
		return (-3.68734834041388E+01 * x - 3.28163398692792E+02) * x + 2.27342862588147E+02;
	} else if (x < 0.7571054399013519) {
		return ((((1.60988309475108E+04 * x - 4.18782706486673E+04) * x + 4.14508040221340E+04) * x - 1.88926043556059E+04) * x + 3.50108270140290E+03) * x - 5.28541997751406E+01;
	} else {
		return 1.68513761929930E+01 * x - 1.06424668227935E+01;
	}
}

vec4 colormapYlOrBr(float x) {
	float r = clamp(colormap_red1(x) / 255.0, 0.0, 1.0);
	float g = clamp(colormap_green1(x) / 255.0, 0.0, 1.0);
	float b = clamp(colormap_blue1(x) / 255.0, 0.0, 1.0);
	return vec4(r, g, b, 1.0);
}
// using colormap from : https://github.com/kbinani/colormap-shaders/
void main()
{

  float x;


  color = colormapJet((vit.y+1)/2);

  //color =   color = vec4(1.0, 0.0,0.0,1.0);
};
