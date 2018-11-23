#version 330 core
//in vec2 vTexCoords;
//in vec3 vNormal;
in vec3 FragPos;

out vec4 color;


float Random2D(vec3 st)
{
    return fract(sin(dot(st.xyz, vec3(12.9898, 78.233, 57.152))) * 43758.5453123);
}

float Interpolate(float a, float b, float x) {  // cosine interpolation
    float ft = x * 3.1415927f;
    float f = (1. - cos(ft)) * 0.5;
    return  a * (1. - f) + b * f;
}

float InterpolatedNoise(int ind, float x, float y, float z) {
    int integer_X = int(floor(x));
    float fractional_X = fract(x);
    int integer_Y = int(floor(y));
    float fractional_Y = fract(y);
    int integer_Z = int(floor(z));
    float fractional_Z = fract(z);
    
    vec3 randomInput = vec3(integer_X, integer_Y, integer_Z);
    float v1 = Random2D(randomInput + vec3(0.0, 0.0, 0.0));
    float v2 = Random2D(randomInput + vec3(1.0, 0.0, 0.0));
    float v3 = Random2D(randomInput + vec3(0.0, 1.0, 0.0));
    float v4 = Random2D(randomInput + vec3(1.0, 1.0, 0.0));
    
    float v5 = Random2D(randomInput + vec3(0.0, 0.0, 1.0));
    float v6 = Random2D(randomInput + vec3(1.0, 0.0, 1.0));
    float v7 = Random2D(randomInput + vec3(0.0, 1.0, 1.0));
    float v8 = Random2D(randomInput + vec3(1.0, 1.0, 1.0));
    
    
    float i1 = Interpolate(v1, v2, fractional_X);
    float i2 = Interpolate(v3, v4,  fractional_X);
    
    float i3 = Interpolate(v5, v6, fractional_X);
    float i4 = Interpolate(v7, v8, fractional_X);
    
    float y1 = Interpolate(i1, i2, fractional_Y);
    float y2 = Interpolate(i3, i4, fractional_Y);
    
    
    return Interpolate(y1, y2, fractional_Z);
}

float perlin(float x, float y, float z){
    
    int numOctaves = 10;
    float persistence = 0.6;
    float total = 0.,
        frequency = .01,
        amplitude = 2.0;
    for (int i = 0; i < numOctaves; ++i) {
        frequency *= 2.;
        amplitude *= persistence;
        
        total += InterpolatedNoise(0, x * frequency, y * frequency, z * frequency) * amplitude;
    }
    return pow(total, 1.0);
}

void main()
{
    
    float value = perlin(FragPos.x, FragPos.y, iTime);
    
    float e = 2.71828182846;
    float y = FragPos.y/iResolution.y;
    value *= ( 1.  - pow(e, -25.0*y))*pow(e, -10.*y);
    
    
    float sigma = 200.;
    float mean = float(iResolution.x/3.);
    float gauss = 1./pow(2.*3.14*sigma*sigma, 0.5)*pow(e, -0.5*pow( ( float(FragPos.x) - mean)/(sigma)  ,2.0));
      
    float value1 = value*sigma*gauss;
    
    float sigma2 = 300.;
    float mean2 = float(iResolution.x*3.0/4.0);
    float gauss2 = 1./pow(2.*3.14*sigma2*sigma2, 0.5)*pow(e, -0.5*pow( ( float(FragPos.x) - mean2)/(sigma2)  ,2.0));
    
    float value2 = value*sigma2*gauss2;
    
    
    if(value1 < 0.15)
        value1 = 0.;
    if(value2 < 0.15)
        value2 = 0.;
    
    сolor = vec4(value1 + value2);
}