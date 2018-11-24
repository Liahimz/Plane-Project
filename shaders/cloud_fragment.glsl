#version 330 core

out vec4 FragColor;

in vec4 col;
in vec3 FragPos;



void main()
{   
    //float c = cnoise(FragPos);
    float color = col.x + 0.4;
    vec4 tex_col = vec4(color,color,color,0.8);
    if (color <= 0.3) {
        tex_col.x += 0.5; tex_col.y += 0.5; tex_col.z += 0.5;
        tex_col.w = 0.4;
    }
    FragColor = tex_col;
}