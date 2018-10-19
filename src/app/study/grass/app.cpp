#include "app.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/renderer/types.h>
#include <imgui/imgui.h>
#include "config.h"

using namespace engine;

namespace app::study::grass {
App::App() : IPlatformApp( { glm::ivec2(WIN_WIDTH, WIN_HEIGHT),
                                                 WIN_TITLE,
                                                 glm::ivec2(WIN_POS_X, WIN_POS_Y),
                                                 USE_GUI, 
                                                 FIXED_FPS, 
                                                 FIXED_DT } )
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
    render_device_.SetViewport(glm::ivec2(0, 0), IPlatformApp::configuration.win_size);
    
    camera_ = std::make_unique<core::Camera>(tp::Vec3(0.0, 0.0, -5.0), (tp::Real)180.0f, (tp::Real)0.0f);
    camera_->SetSpeed(1.4);
    z_near_ = 0.1;
    z_far_ = 100.0;

    // Uniforms
    uniform_buffer_scene_ = std::make_unique<renderer::gl::Buffer>(render_device_.CreateUniformBuffer(sizeof(ShaderData), renderer::RenderDevice::UniformBufferBindingPoint::kScene));
    // Update uniform buffer data
    shader_data_.projection_from_view = glm::perspective(glm::radians(camera_->GetFOV()), IPlatformApp::configuration.win_size.x / (tp::Real)IPlatformApp::configuration.win_size.y, z_near_, z_far_);
    uniform_buffer_scene_->SubData(offsetof(ShaderData, projection_from_view), sizeof(ShaderData::projection_from_view), &shader_data_.projection_from_view[0]);

    // Shader
    std::string  vs_source = std::string(SHADER_PATH) + "grass.vert";
    std::string  gs_source = std::string(SHADER_PATH) + "grass.geom";
    std::string  ps_source = std::string(SHADER_PATH) + "grass.frag";
    std::vector<renderer::gl::Shader> shaders;
    shaders.push_back(render_device_.CreateVertexShader(vs_source));
    shaders.push_back(render_device_.CreateGeometryShader(gs_source));
    shaders.push_back(render_device_.CreatePixelShader(ps_source));
    pipeline_ = std::make_unique<renderer::gl::Program>(render_device_.CreatePipeline(shaders));

    // Plants
    grass_blade_ = std::make_unique<GrassBlade>(render_device_);
    wind_ = std::make_unique<Wind>(render_device_);
}

void App::Update(const platform::Timer &timer, const platform::Input &input) {
    tp::Real dt = timer.VirtualDt();
    // Camera controls using active keys (keys neing currently held)
    if (input.keyboard.active[platform::Input::Key::kW])
        camera_->ProcessKeyboard(core::Camera::kForward, dt);
    if (input.keyboard.active[platform::Input::Key::kS])
        camera_->ProcessKeyboard(core::Camera::kBackward, dt);
    if (input.keyboard.active[platform::Input::Key::kA])
        camera_->ProcessKeyboard(core::Camera::kLeft, dt);
    if (input.keyboard.active[platform::Input::Key::kD])
        camera_->ProcessKeyboard(core::Camera::kRight, dt);
    if (input.keyboard.active[platform::Input::Key::kR])
        camera_->ProcessKeyboard(core::Camera::kUp, dt);
    if (input.keyboard.active[platform::Input::Key::kF])
        camera_->ProcessKeyboard(core::Camera::kDown, dt);

    wind_->Update(dt);
    grass_blade_->Update(dt, *wind_);
}

void App::Render() {
    render_device_.Clear(renderer::RenderDevice::ClearBuffer::kColorDepth);
    // Transforms
    shader_data_.view_from_world = camera_->GetViewMatrix();
    uniform_buffer_scene_->SubData(offsetof(ShaderData, view_from_world), sizeof(ShaderData::view_from_world), &shader_data_.view_from_world[0]);
    pipeline_->Use();
    grass_blade_->Render(render_device_, *uniform_buffer_scene_, shader_data_, tp::Vec3(0.0, 0.0, 0.0));
    wind_->Render(render_device_,*uniform_buffer_scene_, shader_data_);
}

// NOTE: actual rendering takes place after Render function, this only sets up what to render and how it reacts 
void App::RenderGUI(const platform::Timer &timer) {
    ImGui::Text("Loading stats");
    ImGui::Text("Scene loading time (ms): %lu", loading_clock_.GetDuration());
    ImGui::Separator();
    ImGui::Text("Timer stats");
    ImGui::Text("Real time %.3f, virtual %.3f, diff %.3f", timer.RealTime(), timer.VirtualTime(), timer.RealTime() - timer.VirtualTime());
    ImGui::Text("Real dt %.3f, virtual dt %.3f, FPS %.3f", timer.RealDt(), timer.VirtualDt(), (1 / timer.RealDt()));
    ImGui::Text("Updates per frame %u, accum %.4f", timer.updates_per_frame, timer.VirtualAccum());
    ImGui::Separator();
    ImGui::Text("Frame stats");
    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

    wind_->RenderGui();
    grass_blade_->RenderGui();
}

void App::Resize(const glm::ivec2 &size) {
    // height will be significantly larger than specified on retina displays.
    IPlatformApp::configuration.win_size = size;
    render_device_.SetViewport(glm::ivec2(0, 0), IPlatformApp::configuration.win_size);
    // Update uniform buffer data
    shader_data_.projection_from_view = glm::perspective(glm::radians(camera_->GetFOV()), IPlatformApp::configuration.win_size.x / (tp::Real)IPlatformApp::configuration.win_size.y, z_near_, z_far_);
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
        shader_data_.projection_from_view = glm::perspective(glm::radians(camera_->GetFOV()), IPlatformApp::configuration.win_size.x / (tp::Real)IPlatformApp::configuration.win_size.y, z_near_, z_far_);
        uniform_buffer_scene_->SubData(offsetof(ShaderData, projection_from_view), sizeof(ShaderData::projection_from_view), &shader_data_.projection_from_view[0]);
    }
}
};