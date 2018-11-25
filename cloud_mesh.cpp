#include "cloud_mesh.h"
#include "perlin_noise.h"
#include <stdio.h>

CloudMesh* CreateCloudMesh() 
{
    vector<float> pos {
        
        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f,  

        -0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f, 
        
    };
    
    vector<uint32_t> ind {

        0,  1,  2,  3,  4,  5, 
        6,  7,  8,  9,  10, 11, 
        12, 13, 14, 15, 16, 17, 
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35
        
    };


    return new CloudMesh(pos, ind, -1, "Clouds");
}

int InitClouds()
{
    int count = 0;
    for (int k = 0; k < max_clouds; ++k) {
        float3 core_position[5];
        int r1[5], r2[5], r3[5];

        core_position[0].x = -300.0 + (rand() % 700);
        core_position[0].y = 150.0 + (rand() % 100);
        core_position[0].z = -300.0 + (rand() % 700);

        float voxel_size = 0.1;

        r1[0] = 10 + rand() % 6;
        r2[0] = 10 + rand() % 6;
        r3[0] = 1 + rand() % 4;

        for (int l = 1; l < 5; l++) {
            core_position[l].x = core_position[0].x + (5 + (rand() % 3)) * (2 * (rand() % 2) - 1);
            core_position[l].y = core_position[0].y + (5 + (rand() % 3)) * (2 * (rand() % 2) - 1);
            core_position[l].z = core_position[0].z + (3 + (rand() % 2)) * (2 * (rand() % 2) - 1);

            r1[l] = 10 + rand() % 5;
            r2[l] = 10 + rand() % 5;
            r3[l] = 1 + rand() % 4;
        }

        clouds[k].cloud_part_num = 0;

        for (int l = 0; l < 5; l++) {
            for (int i = 0; i < r1[l]; i++) {
                for (int j = 0; j < r2[l]; j++) {
                    for (int z = 0; z < r3[l]; z++) {
                        float col = (PerlinNoise_2D(i, j, 5.0, 20.0) + PerlinNoise_2D(i, z, 1.0, 20.0) + PerlinNoise_2D(j, z, 1.0, 20.0)) / 3.0;
                        if (col <= 0.05) {
                            continue;
                        }
                        //printf ("%f\n",col);
                        clouds[k].position[clouds[k].cloud_part_num].x = core_position[l].x + i; 
                        clouds[k].position[clouds[k].cloud_part_num].y = core_position[l].y + z; 
                        clouds[k].position[clouds[k].cloud_part_num].z = core_position[l].z + j; 

                        float4x4 model_cube = transpose(translate4x4(clouds[k].position[clouds[k].cloud_part_num]));

                        float4x4 matrix = transpose(scale4x4(float3(voxel_size,voxel_size,voxel_size)));

                        model_cube = mul(model_cube,matrix);

                        colors[count] = col;

                        cloud_part_offset[count] = model_cube;
                        count++;
                        clouds[k].cloud_part_num++;
                    }
                }
            }
        }
    }
    return count;
}

void CloudMesh::UpdateClouds(float deltaTime)
{
    int index = 0;
    for (int i = 0; i < max_clouds; ++i) {
        for (int j = 0; j < clouds[i].cloud_part_num; ++j) {
            clouds[i].position[j].x -= 3.0 * deltaTime;
            if (clouds[i].position[j].x <= -300.0) {
                clouds[i].position[j].x = 300.0;
            }
            cloud_part_offset[index + j] = transpose(translate4x4(clouds[i].position[j]));
        }
        index += clouds[i].cloud_part_num;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboOffset);
    glBufferData(GL_ARRAY_BUFFER, instances_num * 16 * sizeof(GL_FLOAT), cloud_part_offset, GL_STREAM_DRAW);
}


CloudMesh::CloudMesh(const vector<float> &positions,
            const vector<uint32_t> &indices,
            size_t mat_id,
            string n) 
{
    instances_num = InitClouds(); //GL_CHECK_ERRORS;

    name = n;

    ind_num = indices.size();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vboVertices);
    glGenBuffers(1, &vboIndices);
    glGenBuffers(1, &vboOffset);
    glGenBuffers(1, &vboColor);

    glBindVertexArray(vao); //GL_CHECK_ERRORS;
    {
        //передаем в шейдерную программу атрибут координат вершин
        glBindBuffer(GL_ARRAY_BUFFER, vboVertices); //GL_CHECK_ERRORS;
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GL_FLOAT), positions.data(), GL_STATIC_DRAW); //GL_CHECK_ERRORS;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); //GL_CHECK_ERRORS;
        glEnableVertexAttribArray(0); //GL_CHECK_ERRORS;

        glBindBuffer(GL_ARRAY_BUFFER, vboColor);
        glBufferData(GL_ARRAY_BUFFER, instances_num * sizeof(float), colors, GL_STREAM_DRAW);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GL_FLOAT), (void*)(0)); //GL_CHECK_ERRORS;
        glEnableVertexAttribArray(1); //GL_CHECK_ERRORS;

        glBindBuffer(GL_ARRAY_BUFFER, vboOffset);
        glBufferData(GL_ARRAY_BUFFER, instances_num * 16 * sizeof(GL_FLOAT), cloud_part_offset, GL_STREAM_DRAW);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float4), (void*)(0)); //GL_CHECK_ERRORS;
        glEnableVertexAttribArray(3); //GL_CHECK_ERRORS;
        
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float4), (void*)(sizeof(float4)));
        glEnableVertexAttribArray(4);

        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float4), (void*)(2 * sizeof(float4)));
        glEnableVertexAttribArray(5);

        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float4), (void*)(3 * sizeof(float4)));
        glEnableVertexAttribArray(6);
        
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        //передаем в шейдерную программу индексы
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices); //GL_CHECK_ERRORS;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW); //GL_CHECK_ERRORS;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0); //GL_CHECK_ERRORS;

    glBindVertexArray(0); //GL_CHECK_ERRORS;

    material_id = mat_id;
}

string CloudMesh::GetName() 
{
    return name;
}

void CloudMesh::Draw() 
{
    glBindVertexArray(vao); //GL_CHECK_ERRORS;
    glDrawElements(GL_TRIANGLES, ind_num, GL_UNSIGNED_INT, nullptr); //GL_CHECK_ERRORS;
    glBindVertexArray(0); //GL_CHECK_ERRORS;
}

void CloudMesh::DrawInstanced() 
{
    glBindVertexArray(vao); //GL_CHECK_ERRORS;
    glDrawElementsInstanced(GL_TRIANGLES, ind_num, GL_UNSIGNED_INT, nullptr, instances_num); //GL_CHECK_ERRORS;
    glBindVertexArray(0); //GL_CHECK_ERRORS;
}

CloudMesh::~CloudMesh() 
{
    glDeleteVertexArrays(1, &vao);
}