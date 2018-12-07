#pragma once

#include "common.h"
#include "LiteMath.h"

#include <vector>
#include <string>

using namespace LiteMath;
using namespace std;

const int max_cloud_parts = 4000;
const int max_clouds = 80;

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
    size_t ind_num, instances_num;
};

CloudMesh* CreateCloudMesh();

int InitClouds();


typedef struct Cloud {
    int cloud_part_num;
    float3 position[max_cloud_parts];
} Cloud;

static Cloud clouds[max_clouds];
static float4x4 cloud_part_offset[max_cloud_parts * max_clouds];
static float colors[max_cloud_parts * max_clouds];

