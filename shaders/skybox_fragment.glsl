#version 330 core
//out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 brightColor;

void main()
{    
    FragColor = texture(skybox, TexCoords);
    brightColor = vec4(0.0);
}