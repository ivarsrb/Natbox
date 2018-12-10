#include "wind_renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <engine/util/math.h>
#include "config.h"

namespace app::study::grass {

WindRenderer::WindRenderer() {
    // Wind vector geometry
    std::vector<engine::renderer::types::FPos> vertices;
    vertices.push_back({glm::vec3(0.0,-0.001,1.0)});
    vertices.push_back({glm::vec3(0.1,-0.001,0.0)});
    vertices.push_back({glm::vec3(-0.1,-0.001,0.0)});
    std::vector<uint32_t> indices;
    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);
    vector_mesh_.SetData(vertices, indices);
    // Shaders
    std::string  vs_source = std::string(SHADER_PATH) + "wind.vert";
    std::string  ps_source = std::string(SHADER_PATH) + "wind.frag";
    std::vector<engine::renderer::gl::Shader> shaders;
    shaders.push_back(engine::renderer::RenderDevice::CreateVertexShader(vs_source));
    shaders.push_back(engine::renderer::RenderDevice::CreatePixelShader(ps_source));
    pipeline_ .emplace(engine::renderer::RenderDevice::CreatePipeline(shaders));
}

void WindRenderer::Render(const WindEntity &wind_entity, engine::renderer::gl::Buffer &uniform_buffer, UniformData &uniform_data) {
    auto rotation_angle = wind_entity.GetSphericVectorFromPosition(engine::tp::Vec3(0)).theta;
    uniform_data.world_from_local = glm::rotate(glm::tmat4x4<engine::tp::Real>(1.0), rotation_angle, engine::util::math::kGroundNormal);
    uniform_buffer.SubData(offsetof(UniformData, world_from_local), sizeof(UniformData::world_from_local), &uniform_data.world_from_local[0]);
    pipeline_->Use();
    vector_mesh_.Render();
}

void WindRenderer::RenderGUI(WindEntity &wind_entity) {
    ImGui::Begin("Wind");
    auto c_vector = wind_entity.GetVectorFromPosition(engine::tp::Vec3(0.0));
    ImGui::Text("Vector: x  %0.2f, y %0.2f, z %0.2f", c_vector.x, c_vector.y, c_vector.z);
    auto s_vector = wind_entity.GetSphericVectorFromPosition(engine::tp::Vec3(0.0));
    float wind_speed = (float)s_vector.radius;
    ImGui::SliderFloat("Speed ", &wind_speed, 0.0f, 15.0f);
    wind_entity.SetSpeed(wind_speed);
    float rot_angle = (float)s_vector.theta;
    ImGui::SliderFloat("Rotation (rad)", &rot_angle, -M_PI, 2*M_PI);
    wind_entity.SetRotation(rot_angle);
    if (ImGui::Button(" Reset ")) {
       wind_entity.Reset();
    }
    ImGui::SameLine();
    if (ImGui::Button(" Stop ")) {
       wind_entity.Stop();
    }
    
    ImGui::End();
}

}; // grass