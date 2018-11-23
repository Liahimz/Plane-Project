#include "drawcall.h"
#include <iostream>
#include <sstream>
#include <string>

void DrawSimpleTriangle(ShaderProgram &debug_program,
    Camera &camera,
    uint32_t width,
    uint32_t height) 
{
    debug_program.StartUseShader();
    DrawTriangle();
    debug_program.SetUniform("projection", projectionMatrixTransposed(90.0f, float(width) / float(height), 1.1f, 100));
    float3 t = make_float3(0.0f, 0.0f, 15.0f);
    debug_program.SetUniform("view", camera.GetViewMatrix());
    debug_program.StopUseShader();
}

void DrawRain(ShaderProgram &program,
    Camera &camera,
    RainMesh *mesh,
    uint32_t width,
    uint32_t height,
    float deltaTime) {

    program.StartUseShader();
    program.SetUniform("view", camera.GetViewMatrix());
    program.SetUniform("projection", projectionMatrixTransposed(90.0, float(width) / float(height), 0.1f, 1000.0f));

    mesh->update_particles(deltaTime);
    mesh->DrawInstanced(MAX_RAINDROPS);
    program.StopUseShader();
}

void do_a_barrel_roll()
{
    if (PLANE_BARREL_ROLLING == false) {
        PLANE_BARREL_ROLLING = true;

        for (int i = 0; i < plane_num; ++i) {
            rotates_lates[i] = 0.0;//(0.1 + (float(rand()) / RAND_MAX) * 0.7);
            rotates_speed[i] = 3.0;//(0.8 + (float(rand()) / RAND_MAX) * 0.2);

            planes_rotate_angles[i] = -rotates_lates[i];
            is_rotates_completed[i] = false;
        }

        //std::cout << "do a roll" << std::endl;
    }
}

void DrawMesh(ShaderProgram &program,
              Camera &camera,
              std::unique_ptr<Mesh> &mesh,
              uint32_t width,
              uint32_t height,
              float deltaTime) 
{
    program.StartUseShader();

    program.SetUniform("color_texture", 0);

    program.SetUniform("view", camera.GetViewMatrix());
    program.SetUniform("viewPos", camera.pos);
    program.SetUniform("projection", projectionMatrixTransposed(camera.zoom, float(width) / float(height), 0.1f, 1000.0f));
    //отражние
    program.SetUniform("skybox", 2);

    if (mesh->GetName() == "Aircraft_propeller") {
        const float ROTATES_RATE = 10.0;
        mesh->model = mul(rotate_X_4x4(deltaTime * ROTATES_RATE), mesh->model);
    }
    program.SetUniform("model", mesh->model);


    // planes instancing
    const float offset = 7.0f;
    float4x4 offsets[plane_num];
    // ===================


    // PLANES LOCAL SHIFTS
    const float left_border  = -0.7;
    const float right_border =  0.7;

    static float3 last_shifts[plane_num] = {float3(0,0,0), };
    static float3 directions[plane_num]  = {float3(0, 0, 0) };
    // ===============

    for(int i = -5, j = 0; i <= 5; ++i, ++j) {
        float3 translation = float3(
            -abs(i * 2.3 * offset),
            0,
            i * 1.2 * offset
        );

        if (directions[j].x == 0 || last_shifts[j].x < left_border || last_shifts[j].x > right_border) {
            if (directions[j].x < 0) {
                directions[j].x = 1.0;
            } else if (directions[j].x > 0) {
                directions[j].x = -1.0;
            } else {
                if (rand() % 2)
                    directions[j].x = -1.0;
                else
                    directions[j].x =  1.0;
            }
        }

        if (directions[j].y == 0 || last_shifts[j].y < left_border || last_shifts[j].y > right_border) {
            if (directions[j].y < 0) {
                directions[j].y = 1.0;
            } else if (directions[j].y > 0) {
                directions[j].y = -1.0;
            } else {
                if (rand() % 2)
                    directions[j].y = -1.0;
                else
                    directions[j].y =  1.0;
            }
        }

        if (directions[j].z == 0 || last_shifts[j].z < left_border || last_shifts[j].z > right_border) {
            if (directions[j].z < 0) {
                directions[j].z = 1.0;
            } else if (directions[j].z > 0) {
                directions[j].z = -1.0;
            } else {
                if (rand() % 2)
                    directions[j].z = -1.0;
                else
                    directions[j].z =  1.0;
            }
        }

        last_shifts[j] += (directions[j] * deltaTime) / 30.0 * (1.0 + float(rand()) / RAND_MAX * 2.5);

        // translation.y = abs((float)i * offset);
        offsets[j] = transpose(translate4x4(translation + last_shifts[j]));

        // BARREL ROLLING
        if (!is_rotates_completed[j]) {
            planes_rotate_angles[j] += deltaTime * rotates_speed[j] / 2.0;
            if (planes_rotate_angles[j] >= 2 * 3.1415) {

                planes_rotate_angles[j] = 0.0;
                is_rotates_completed[j] = true;

                bool all_ends = true;
                for (int k = 0; k < plane_num; ++k) {
                    if (!is_rotates_completed[k])
                        all_ends = false;
                }

                if (all_ends) {
                    PLANE_BARREL_ROLLING = false;
                }
            }

            if (planes_rotate_angles[j] > 0.0) {
                offsets[j] = mul(rotate_X_4x4(planes_rotate_angles[j]), offsets[j]);
            }
        }
    }


    // draw planes
    for(unsigned int i = 0; i < plane_num; i++) {
        std::stringstream ss;
        string index;
        ss << i;
        index = ss.str();
        program.SetUniform(("offsets[" + index + "]").c_str(), offsets[i]);
    }

    mesh->DrawInstanced(plane_num);

    program.StopUseShader();
}