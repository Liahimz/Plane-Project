#pragma once

#include "common.h"
#include "LiteMath.h"

#include <vector>
#include <cstring>

using LiteMath::float4x4;
using LiteMath::float3;
using LiteMath::float4;
using LiteMath::rotate_Y_4x4;
using LiteMath::mul;

using std::vector;
using std::string;


class CloudMesh {
public:
    float4x4 model;
    uint32_t material_id;

    CloudMesh(const vector<float> &positions,
             const vector<uint32_t> &indices,
             size_t mat_id,
             string n);

    string GetName();

    void Draw();

    void DrawInstanced();

    void UpdateClouds(float deltaTime);

    ~CloudMesh();

protected:
    string name;
    GLuint vboVertices, vboIndices, vboOffset, vboColor, vao;
    size_t ind_num;
};


// ========== CLOUD PART ===============
CloudMesh* CreateCloudMesh();


static void reset_cloud(int index);
static void init_clouds();


static const int MAX_CLOUD_PARTICLES = 4000;
static const int MAX_CLOUDS = 80;

static const int MIN_HEIGHT = 200; // 200
static const int MAX_HEIGHT = 300; // 300

static const int DRAW_RANGE = 600; // 2000

static const int MIN_LAYERS = 2;
static const int MAX_LAYERS = 6;

static const float zoom = 2.0;


typedef struct Cloud {
    int particles_count;

    // Position/direction
    float3 position;
    float3 particles_position[MAX_CLOUD_PARTICLES];
    float rotate;

    // Velocity
    float velocity;
} Cloud;


static int TOTAL_PARTICLES = 0;

static Cloud clouds[MAX_CLOUDS];
static float4x4 cloud_particles_offset[MAX_CLOUD_PARTICLES * MAX_CLOUDS];
static float4 colors[MAX_CLOUD_PARTICLES * MAX_CLOUDS];
// ====================================