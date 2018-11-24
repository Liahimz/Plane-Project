#include "cloud_mesh.h"


// ============= NOISE ================
static float Noise(float x, float y) {
    int n = int(x + y * 57);
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

static float SmoothedNoise(float x, float y) {
    float corners = (Noise(x-1, y-1) + Noise(x+1, y-1) + Noise(x-1, y+1) + Noise(x+1, y+1)) / 16;
    float sides   = (Noise(x-1, y) + Noise(x+1, y) + Noise(x, y-1) + Noise(x, y+1)) /  8;
    float center  =  Noise(x, y) / 4;
    return corners + sides + center;
}

static float Interpolate(float a, float b, float x) {
    float ft = x * 3.1415927;
    float f = (1 - cos(ft)) * 0.5;
    return  a*(1-f) + b*f;
}


static float InterpolatedNoise(float x, float y) {

      int integer_X    = int(x);
      float fractional_X = x - integer_X;

      int integer_Y    = int(y);
      float fractional_Y = y - integer_Y;

      float v1 = SmoothedNoise(integer_X,     integer_Y);
      float v2 = SmoothedNoise(integer_X + 1, integer_Y);
      float v3 = SmoothedNoise(integer_X,     integer_Y + 1);
      float v4 = SmoothedNoise(integer_X + 1, integer_Y + 1);

      float i1 = Interpolate(v1 , v2 , fractional_X);
      float i2 = Interpolate(v3 , v4 , fractional_X);

      return Interpolate(i1 , i2 , fractional_Y);
}

static float PerlinNoise_2D(float x, float y, float persistence, float Number_Of_Octaves) {

      float total = 0;

      float p = persistence;
      float n = Number_Of_Octaves - 1;

      for (float i = 0.0; i < n; i+=1.0) {

        float frequency = pow(2,i);
        float amplitude= pow(p,i);

        total = total + InterpolatedNoise(x * frequency, y * frequency) * amplitude;

      }

      return total;

}
// ====================================


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

    // clear all buffers
    memset(clouds, 0, sizeof(clouds));
    memset(cloud_particles_offset, 0, sizeof(clouds));
    memset(colors, 0, sizeof(colors));
    TOTAL_PARTICLES = 0;

    init_clouds();

    for (int i = 0; i < pos.size(); ++i) {
        pos[i] *= zoom;
    }
    return new CloudMesh(pos, ind, -1, "Clouds");
}


static void reset_cloud(int index)
{
    clouds[index].velocity = -10.0;
    clouds[index].position = float3(
        -DRAW_RANGE + (rand() % (2 * DRAW_RANGE)),
        MIN_HEIGHT + (rand() % (MAX_HEIGHT - MIN_HEIGHT + 1)),
        -DRAW_RANGE + (rand() % (2 * DRAW_RANGE))
    );


    float particle_size = 0.7 * zoom;

    // generate cloud shape
    float max_cloud_x, max_cloud_z;
    float proportion = 0.5 + (float(rand()) / float(RAND_MAX));
    int y_border = MIN_LAYERS + rand() % (MAX_LAYERS - MIN_LAYERS + 1);

    max_cloud_x = sqrt(((float(MAX_CLOUD_PARTICLES) / y_border) / proportion));
    max_cloud_z = max_cloud_x * proportion;

    // std::cout << "max_cloud_x = " << max_cloud_x << "\n";
    // std::cout << "max_cloud_z = " << max_cloud_z << "\n";

    float zoom_coef = 0.4;
    int x_border = int(max_cloud_x * zoom_coef) + (rand() % int(max_cloud_x)) * (1.0 - zoom_coef);
    int z_border = int(max_cloud_z * zoom_coef) + (rand() % int(max_cloud_z)) * (1.0 - zoom_coef);

    // std::cout << "create new cloud:\n";

    for (int x = 0; x < x_border; ++x) {
        for (int y = 0; y < y_border; ++y) {
            for (int z = 0; z < z_border; ++z) {
                float col = (PerlinNoise_2D(x, y, 0.9, 15.0) + PerlinNoise_2D(x, z, 0.9, 15.0) + PerlinNoise_2D(y, z, 0.9, 15.0)) / 3.0 + 0.2;

                if (col <= 0.3) {
                    // particle doesn't exist
                    continue;
                }


                int particle_index = clouds[index].particles_count;
                clouds[index].particles_position[particle_index] = clouds[index].position + float3(x, y, z) * particle_size;

                // std::cout << "\tcreate new particle\n";
                // std::cout << "position for particle = (" << clouds[index].particles_position[particle_index].x << " ,";
                // std::cout << clouds[index].particles_position[particle_index].y << ", ";
                // std::cout << clouds[index].particles_position[particle_index].z << ");\n)";

                float4x4 model_cube = transpose(translate4x4(clouds[index].particles_position[particle_index]));

                //float4x4 matrix = transpose(scale4x4(float3(particle_size,particle_size,particle_size)));

                //model_cube = mul(matrix,model_cube);

                colors[TOTAL_PARTICLES] = float4(col, col, col, 1.0);
                cloud_particles_offset[TOTAL_PARTICLES] = model_cube;
                clouds[index].particles_count += 1;
                TOTAL_PARTICLES += 1;
            }
        }
    }
}


static void init_clouds()
{
    for (int i = 0; i < MAX_CLOUDS; ++i) {
        reset_cloud(i);
    }
    // std::cout << "TOTAL_PARTICLES = " << TOTAL_PARTICLES << std::endl;
}


void CloudMesh::UpdateClouds(float deltaTime)
{
    TOTAL_PARTICLES = 0;
    for (int i = 0; i < MAX_CLOUDS; ++i) {
        for (int j = 0; j < clouds[i].particles_count; ++j) {
            clouds[i].particles_position[j].x += clouds[i].velocity * deltaTime;
            // if (clouds[i].particles_position[j].x <= -DRAW_RANGE) {
            //     TOTAL_PARTICLES -= j;
            //     reset_cloud(i);
            //     break;
            // } else {
            //     float4x4 model_cube = transpose(translate4x4(clouds[i].particles_position[j]));
            //     cloud_particles_offset[TOTAL_PARTICLES] = model_cube;
            //     TOTAL_PARTICLES += 1;
            // }
            float4x4 model_cube = transpose(translate4x4(clouds[i].particles_position[j]));
            cloud_particles_offset[TOTAL_PARTICLES] = model_cube;
            TOTAL_PARTICLES += 1;
            
        }
    }

    //std::cout << TOTAL_PARTICLES << std::endl;

    // just for tests
    glBindBuffer(GL_ARRAY_BUFFER, vboOffset);
    glBufferData(GL_ARRAY_BUFFER, TOTAL_PARTICLES * 16 * sizeof(GL_FLOAT), cloud_particles_offset, GL_STREAM_DRAW);

    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float4), (void*)(0)); /*GL_CHECK_ERRORS;*/
    glEnableVertexAttribArray(3); /*GL_CHECK_ERRORS;*/

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
}


CloudMesh::CloudMesh(const vector<float> &positions,
            const vector<uint32_t> &indices,
            size_t mat_id,
            string n)
{
    name = n;

    ind_num = indices.size();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vboVertices);
    glGenBuffers(1, &vboIndices);
    glGenBuffers(1, &vboOffset);
    glGenBuffers(1, &vboColor);

    glBindVertexArray(vao); /*GL_CHECK_ERRORS;*/
    {
        //передаем в шейдерную программу атрибут координат вершин
        glBindBuffer(GL_ARRAY_BUFFER, vboVertices); /*GL_CHECK_ERRORS;*/
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GL_FLOAT), positions.data(), GL_STATIC_DRAW); /*GL_CHECK_ERRORS;*/
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); /*GL_CHECK_ERRORS;*/
        glEnableVertexAttribArray(0); /*GL_CHECK_ERRORS;*/

        // =============== COLORS ====================
        glBindBuffer(GL_ARRAY_BUFFER, vboColor);
        glBufferData(GL_ARRAY_BUFFER, TOTAL_PARTICLES * sizeof(float4), colors, GL_STREAM_DRAW);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)(0)); /*GL_CHECK_ERRORS;*/
        glEnableVertexAttribArray(1); /*GL_CHECK_ERRORS;*/

        // ============= CLOUD PARTICLES ================
        glBindBuffer(GL_ARRAY_BUFFER, vboOffset);
        glBufferData(GL_ARRAY_BUFFER, TOTAL_PARTICLES * 16 * sizeof(GL_FLOAT), cloud_particles_offset, GL_STREAM_DRAW);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float4), (void*)(0)); /*GL_CHECK_ERRORS;*/
        glEnableVertexAttribArray(3); /*GL_CHECK_ERRORS;*/

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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices); /*GL_CHECK_ERRORS;*/
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW); /*GL_CHECK_ERRORS;*/
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0); /*GL_CHECK_ERRORS;*/

    glBindVertexArray(0); /*GL_CHECK_ERRORS;*/

    material_id = mat_id;
}

string CloudMesh::GetName()
{
    return name;
}

void CloudMesh::Draw()
{
    glBindVertexArray(vao); /*GL_CHECK_ERRORS;*/
    glDrawElements(GL_TRIANGLES, ind_num, GL_UNSIGNED_INT, nullptr); /*GL_CHECK_ERRORS;*/
    glBindVertexArray(0); /*GL_CHECK_ERRORS;*/
}

void CloudMesh::DrawInstanced()
{
    glBindVertexArray(vao); /*GL_CHECK_ERRORS;*/
    glDrawElementsInstanced(GL_TRIANGLES, ind_num, GL_UNSIGNED_INT, nullptr, TOTAL_PARTICLES); /*GL_CHECK_ERRORS;*/
    glBindVertexArray(0); /*GL_CHECK_ERRORS;*/
}

CloudMesh::~CloudMesh()
{
    glDeleteVertexArrays(1, &vao);
}