#include "app.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/renderer/types.h>
#include <imgui/imgui.h>
#include "config.h"

using namespace engine;

namespace app::study::terrain {
App::App() : IPlatformApp( { glm::ivec2(WIN_WIDTH, WIN_HEIGHT),
                                                 WIN_TITLE,
                                                 glm::ivec2(WIN_POS_X, WIN_POS_Y),
                                                 USE_GUI } )
{
    loading_clock_.SetTime1();
    Init();
    loading_clock_.SetTime2(false);
}

App::~App() {
    
}

void App::Init() {
    // Depth stencil state
    render_device_.SetDepthStencilState(renderer::RenderDevice::DepthStencilState::kDepth);
    // Opengl state
    render_device_.SetRasterState(renderer::RenderDevice::RasterState::kNoCullSolid);
    // View port
    render_device_.SetViewport(glm::ivec2(0, 0), IPlatformApp::window_size_);
    
    camera_ = std::make_unique<core::Camera>(glm::tvec3<tp::Real>(0.0, 0.0, 0.0), (tp::Real)180.0f, (tp::Real)0.0f);
    camera_->SetSpeed(1.4);
    z_near_ = 0.1;
    z_far_ = 500.0;

    // Uniforms
    uniform_buffer_scene_ = std::make_unique<renderer::gl::Buffer>(render_device_.CreateUniformBuffer(sizeof(ShaderData), renderer::RenderDevice::UniformBufferBindingPoint::kScene));
    // Update uniform buffer data
    shader_data_.projection_from_view = glm::perspective(glm::radians(camera_->GetFOV()), IPlatformApp::window_size_.x / (tp::Real)IPlatformApp::window_size_.y, z_near_, z_far_);
    uniform_buffer_scene_->SubData(offsetof(ShaderData, projection_from_view), sizeof(ShaderData::projection_from_view), &shader_data_.projection_from_view[0]);

    // Shader
    std::string  vs_source = std::string(SHADER_PATH) + "terrain.vert";
    std::string  ps_source = std::string(SHADER_PATH) + "terrain.frag";
    //renderer::gl::Shader vertex_shader = render_device_.CreateVertexShader(vs_source);
    //renderer::gl::Shader pixel_shader = render_device_.CreatePixelShader(ps_source);
    //pipeline_ = std::make_unique<renderer::gl::Program>(render_device_.CreatePipeline(vertex_shader, pixel_shader));
    std::vector<renderer::gl::Shader> shaders;
    shaders.push_back(render_device_.CreateVertexShader(vs_source));
    shaders.push_back(render_device_.CreatePixelShader(ps_source));
    pipeline_ = std::make_unique<renderer::gl::Program>(render_device_.CreatePipeline(shaders));

    // Objects
    terrain_ = std::make_unique<Terrain>(render_device_);
}

void App::Update(const platform::IPlatformApp::Timing *time, const platform::Input *input) {
    // Camera controls using active keys (keys neing currently held)
    if (input->keyboard.active[platform::Input::Key::kW])
        camera_->ProcessKeyboard(core::Camera::kForward, (tp::Real)time->delta);
    if (input->keyboard.active[platform::Input::Key::kS])
        camera_->ProcessKeyboard(core::Camera::kBackward, (tp::Real)time->delta);
    if (input->keyboard.active[platform::Input::Key::kA])
        camera_->ProcessKeyboard(core::Camera::kLeft, (tp::Real)time->delta);
    if (input->keyboard.active[platform::Input::Key::kD])
        camera_->ProcessKeyboard(core::Camera::kRight, (tp::Real)time->delta);
}

void App::Render(const platform::IPlatformApp::Timing *time) {
    render_device_.Clear(renderer::RenderDevice::ClearBuffer::kColorDepth);
    //render_device_.SetRasterState(renderer::RenderDevice::RasterState::kNoCullWire);
    // Transforms
    shader_data_.view_from_world = camera_->GetViewMatrix();
    uniform_buffer_scene_->SubData(offsetof(ShaderData, view_from_world), sizeof(ShaderData::view_from_world), &shader_data_.view_from_world[0]);
    pipeline_->Use();
    shader_data_.world_from_local = glm::mat4(1.0);
    terrain_->Render(render_device_, *uniform_buffer_scene_, shader_data_);

    if (USE_GUI) {
        RenderGui();
    }
}
// Render UI for tests and debugging
// NOTE: actual rendering takes place after Render function, this only sets up what to render and how it reacts 
void App::RenderGui() {
    ImGui::Text("Loading stats");
    ImGui::Text("Scene loading time (ms): %lu", loading_clock_.GetDuration());
    ImGui::Separator();
    ImGui::Text("Frame stats");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Drawcals: %d, primitives: %d", render_device_.render_stats.num_drawcalls_per_frame, render_device_.render_stats.num_primitives_per_frame);
    ImGui::Separator();
    ImGui::Text("Render options");
    bool wireframe = (render_device_.render_stats.previous_raster_state == renderer::RenderDevice::RasterState::kNoCullWire);
    ImGui::Checkbox("Wireframe ", &wireframe); //ImGui::SameLine(150);
    if (wireframe) {
        render_device_.SetRasterState(renderer::RenderDevice::RasterState::kNoCullWire);
    }
    else {
        render_device_.SetRasterState(renderer::RenderDevice::RasterState::kNoCullSolid);
    }
    ImGui::Separator();
    ImGui::Text("Camera");
    ImGui::Text("Poition: x  %0.2f, y %0.2f, z %0.2f", camera_->GetPosition().x, camera_->GetPosition().y, camera_->GetPosition().z);
    float cam_speed = (float)camera_->GetSpeed();
    ImGui::SliderFloat("Speed unit/s ", &cam_speed, 1.4f, 5000.0f);
    camera_->SetSpeed(cam_speed);

    terrain_->RenderGui();
}

void App::Resize(const glm::ivec2 &size) {
    // height will be significantly larger than specified on retina displays.
    IPlatformApp::window_size_ = size;
    render_device_.SetViewport(glm::ivec2(0, 0), IPlatformApp::window_size_);
    // Update uniform buffer data
    shader_data_.projection_from_view = glm::perspective(glm::radians(camera_->GetFOV()), IPlatformApp::window_size_.x / (tp::Real)IPlatformApp::window_size_.y, z_near_, z_far_);
    uniform_buffer_scene_->SubData(offsetof(ShaderData, projection_from_view), sizeof(ShaderData::projection_from_view), &shader_data_.projection_from_view[0]);
}

void App::KeyPress(const platform::Input *input) {
    
}

void App::MouseMove(const platform::Input *input) {
    if (input->mouse.cursor_disabled) {
        camera_->ProcessMouseMovement(input->mouse.offset.x, input->mouse.offset.y);
    }
}

void App::MouseButtonPress(const engine::platform::Input * input) {

}

void App::MouseScroll(const platform::Input *input) {
    if (input->mouse.cursor_disabled) {
        camera_->ProcessMouseScroll(input->mouse.scroll_offset.y);
        // Update uniform buffer data
        shader_data_.projection_from_view = glm::perspective(glm::radians(camera_->GetFOV()), IPlatformApp::window_size_.x / (tp::Real)IPlatformApp::window_size_.y, z_near_, z_far_);
        uniform_buffer_scene_->SubData(offsetof(ShaderData, projection_from_view), sizeof(ShaderData::projection_from_view), &shader_data_.projection_from_view[0]);
    }
}
};