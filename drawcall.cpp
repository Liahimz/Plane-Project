#include "drawcall.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>

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

void DrawClouds(ShaderProgram &program,
        Camera &camera,
        CloudMesh *mesh,
        uint32_t width,
        uint32_t height,
        float deltaTime) {

    program.StartUseShader();

    mesh->UpdateClouds(deltaTime);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    program.SetUniform("view", camera.GetViewMatrix());
    program.SetUniform("projection", projectionMatrixTransposed(camera.zoom, float(width) / float(height), 0.1f, 1000.0f));

    mesh->DrawInstanced();

    glDisable(GL_BLEND);

    program.StopUseShader();
}

void do_a_barrel_roll()
{
    if (PLANE_BARREL_ROLLING == false) {
        PLANE_BARREL_ROLLING = true;

        for (int i = 0; i < plane_num; ++i) {
            rotates_speed[i] = 3.0;

            is_rotates_completed[i] = false;
        }
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

    program.SetUniform("color_texture", 2);

    program.SetUniform("lightPos", float3(10.0f,10.4f,-15.0f));

    program.SetUniform("view", camera.GetViewMatrix());
    program.SetUniform("viewPos", camera.pos);

    program.SetUniform("lightColor", float3(1.0f,1.0f,1.0f));

    program.SetUniform("projection", projectionMatrixTransposed(camera.zoom, float(width) / float(height), 0.1f, 1000.0f));
    program.SetUniform("skybox", 3);

    if (mesh->GetName() == "Aircraft_propeller") {
        const float ROTATES_RATE = 10.0;
        mesh->model = mul(rotate_X_4x4(deltaTime * ROTATES_RATE), mesh->model);
    }
    program.SetUniform("model", mesh->model);


    const float offset = 1.0f;
    float4x4 offsets[plane_num];

    for(int i = -5, j = 0; i <= 5; i++, j++) {
        
        float3 translation = float3(
            -(i * 2.3 * offset) * (i * 2.3 * offset),
            0,
            i * 1.2 * offset * 10
        );

        offsets[j] = transpose(translate4x4(translation));

        if (!is_rotates_completed[j]) {
            planes_rotate_angles[j] += deltaTime * rotates_speed[j] / 2.0;
            if (planes_rotate_angles[j] >= 2 * 3.1415) {

                planes_rotate_angles[j] = 0.0;
                is_rotates_completed[j] = true;
                PLANE_BARREL_ROLLING = false;
                
            }

            if (planes_rotate_angles[j] > 0.0) {
                offsets[j] = mul(rotate_X_4x4(planes_rotate_angles[j]), offsets[j]);
            }
        }
    }


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