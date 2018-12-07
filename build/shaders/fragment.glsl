#version 330 core
in vec2 vTexCoords;
in vec3 vNormal;
in vec3 FragPos;

//out vec4 color;


uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform sampler2D color_texture;
uniform samplerCube skybox;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 brightColor;

void main()
{
	
	//vec3 lightPos = vec3(1.0f,10.4f,-15.0f);
	//vec3 lightColor = vec3(1.0f,1.0f,1.0f);
	//vec3 viewPos = vec3(1.0f,2.0f,2.0f);

    vec4 color_test = texture(color_texture, vTexCoords);

	float ambientStrenght = 0.4f;
	vec3 ambient = ambientStrenght * lightColor;


	vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 3.0f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;


    vec3 light_result = (ambient + diffuse + specular);
    vec4 light_color = vec4(light_result, 1.0f);

    color = texture(color_texture, vTexCoords) * light_color;



    vec4 cabin_color = vec4(37.0/255.0, 47.0/255.0, 43.0/255.0, 1);
    if (color_test.x <= cabin_color.x + 0.02 &&  color_test.x >= cabin_color.x - 0.02 
        && color_test.y <= cabin_color.y + 0.02 && color_test.y >= cabin_color.y - 0.02
        && color_test.z <= cabin_color.z + 0.02 && color_test.z >= cabin_color.z - 0.02) {
        vec3 I = normalize(FragPos - viewPos);
        vec3 R = reflect(I, normalize(vNormal));
        color = vec4(texture(skybox, R).rgb, 1) * light_color;
    }

    vec3 result = color.rgb;

    float brightness = dot(result, vec3(0.2526, 0.7452, 0.0822));
    if (brightness > 1.0) {
        brightColor = vec4(result, 1.0);
    } else {
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

}