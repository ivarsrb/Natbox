#include "wind.h"
#include <imgui/imgui.h>
#include <engine/util/log.h>
#include <glm/gtc/matrix_transform.hpp>
#include "config.h"

using namespace engine;

namespace app::study::grass {
Wind::Wind(renderer::RenderDevice &render_device) {
    Reset();
    InitGeometry(render_device);
}

void Wind::InitGeometry(renderer::RenderDevice &render_device) {
    // Wind vector geometry
    std::vector<renderer::types::FPos> vertices;
    vertices.push_back({glm::vec3(0.0,0.0,1.0)});
    vertices.push_back({glm::vec3(0.1,0.0,0.0)});
    vertices.push_back({glm::vec3(-0.1,0.0,0.0)});
    std::vector<uint32_t> indices;
    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);
    vector_mesh_.SetData(render_device, vertices, indices);
    // Shader
    std::string  vs_source = std::string(SHADER_PATH) + "wind.vert";
    std::string  ps_source = std::string(SHADER_PATH) + "wind.frag";
    std::vector<renderer::gl::Shader> shaders;
    shaders.push_back(render_device.CreateVertexShader(vs_source));
    shaders.push_back(render_device.CreatePixelShader(ps_source));
    pipeline_ = std::make_unique<renderer::gl::Program>(render_device.CreatePipeline(shaders));
}

void Wind::Reset() {
    // Initial direction and strenth in cartesian coordinates system
    tp::Vec3 c_initial_vector_ = tp::Vec3(0.0, 0.0, 1.0);
    // Transform to spherical
    s_vector_.InitFromCartesian(c_initial_vector_); 
}

void Wind::Stop() {
    SetSpeed(0);
}

void Wind::Update(engine::tp::Real dt) {

}

// In spherical coordinates speed is represented by radius 
void Wind::SetSpeed(engine::tp::Real speed) {
    if (speed < 0) {
        speed = 0.0;
        util::Log::Write2("Wind speed is negative; set to 0");
    }    
    s_vector_.radius = speed;
}

// Rotation angle theta of wind vector in spherical coordinates
void Wind::SetRotation(engine::tp::Real angle) {
    s_vector_.theta = angle;
}

// Get wind vector in given position
engine::tp::Vec3 Wind::GetVectorFromPosition(const tp::Vec3 &pos) const {
    // TODO add position sampling here
    return s_vector_.GetCartesian();
}

bool Wind::IsBlowing() {
    return (bool)s_vector_.radius;
}

void Wind::Render(engine::renderer::RenderDevice &render_device, renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data) {
    shader_data.world_from_local = glm::rotate(glm::tmat4x4<tp::Real>(1.0), s_vector_.theta, util::math::kGroundNormal);
    uniform_buffer_scene.SubData(offsetof(ShaderData, world_from_local), sizeof(ShaderData::world_from_local), &shader_data.world_from_local[0]);
    pipeline_->Use();
    vector_mesh_.Render(render_device);
}

// Render UI for tests and debugging
// NOTE: actual rendering takes place after Render function, this only sets up what to render and how it reacts 
void Wind::RenderGui() {
    ImGui::Begin("Wind");
    tp::Vec3 c_vector = GetVectorFromPosition(tp::Vec3(0.0));
    ImGui::Text("Vector: x  %0.2f, y %0.2f, z %0.2f", c_vector.x, c_vector.y, c_vector.z);
    float wind_speed = (float)s_vector_.radius;
    ImGui::SliderFloat("Speed ", &wind_speed, 0.0f, 15.0f);
    SetSpeed(wind_speed);
    float rot_angle = (float)s_vector_.theta;
    ImGui::SliderFloat("Rotation (rad)", &rot_angle, -M_PI, 2*M_PI);
    SetRotation(rot_angle);
    if (ImGui::Button(" Reset ")) {
       Reset();
    }
    ImGui::SameLine();
    if (ImGui::Button(" Stop ")) {
       Stop();
    }
    
    ImGui::End();
}
};
