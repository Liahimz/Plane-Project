#version 330 core
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;


out vec2 vTexCoords;
out vec3 vNormal;
out vec3 FragPos;

uniform mat4 offsets[12];


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	mat4 offset = offsets[gl_InstanceID];
    gl_Position = projection * view * offset * model * vec4(vertex, 1.0f);
    //add offset for instancing
    // Передает текстурные координаты дальше в Fragment шейдер
    vNormal = mat3(transpose(inverse(model))) * normal;
    FragPos = vec3(model * vec4(vertex, 1.0f));
    vTexCoords = texCoords;

}