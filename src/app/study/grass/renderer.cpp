#include "renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

namespace app::study::grass {
Renderer::Renderer(const glm::ivec2 &window_size, const GrassCollection &grass_collection): 
                                            camera_(engine::tp::Vec3(0.0, 1.0, -5.0), 180.0, 0.0),
                                            z_near_ (0.1),
                                            z_far_ (100.0),
                                            uniform_buffer_(render_device_.CreateUniformBuffer(sizeof(UniformData), engine::renderer::RenderDevice::UniformBufferBindingPoint::kScene)),
                                            grass_renderer_(render_device_, grass_collection) ,
                                            wind_renderer_(render_device_)
{
    // Depth stencil state
    render_device_.SetDepthStencilState(engine::renderer::RenderDevice::DepthStencilState::kDepth);
    // Opengl state
    render_device_.SetRasterState(engine::renderer::RenderDevice::RasterState::kNoCullSolid);
    // View port
    render_device_.SetViewport(glm::ivec2(0, 0), window_size);
    
    // projection matrix
    uniform_data_.projection_from_view = glm::perspective(glm::radians(camera_.GetFOV()), window_size.x / (engine::tp::Real)window_size.y, z_near_, z_far_);
    uniform_buffer_.SubData(offsetof(UniformData, projection_from_view), sizeof(UniformData::projection_from_view), &uniform_data_.projection_from_view[0]);
}


void Renderer::Update(const engine::tp::Real dt, const engine::platform::Input &input) {
    // TODO: controler should be separate from renderer
    // Camera controls using active keys (keys neing currently held)
    if (input.keyboard.active[engine::platform::Input::Key::kW])
        camera_.ProcessKeyboard(engine::core::Camera::kForward, dt);
    if (input.keyboard.active[engine::platform::Input::Key::kS])
        camera_.ProcessKeyboard(engine::core::Camera::kBackward, dt);
    if (input.keyboard.active[engine::platform::Input::Key::kA])
        camera_.ProcessKeyboard(engine::core::Camera::kLeft, dt);
    if (input.keyboard.active[engine::platform::Input::Key::kD])
        camera_.ProcessKeyboard(engine::core::Camera::kRight, dt);
    if (input.keyboard.active[engine::platform::Input::Key::kR])
        camera_.ProcessKeyboard(engine::core::Camera::kUp, dt);
    if (input.keyboard.active[engine::platform::Input::Key::kF])
        camera_.ProcessKeyboard(engine::core::Camera::kDown, dt);
}


void Renderer::Render(const GrassCollection &grass_collection, const WindEntity &wind_entity) {
    render_device_.Clear(engine::renderer::RenderDevice::ClearBuffer::kColorDepth);

    // View matrix
    uniform_data_.view_from_world = camera_.GetViewMatrix();
    uniform_buffer_.SubData(offsetof(UniformData, view_from_world), sizeof(UniformData::view_from_world), &uniform_data_.view_from_world[0]);

    grass_renderer_.Render(render_device_, grass_collection, uniform_buffer_, uniform_data_);
    wind_renderer_.Render(render_device_, wind_entity, uniform_buffer_, uniform_data_);
}

// NOTE: actual rendering takes place after Render function, this only sets up what to render and how it reacts 
void Renderer::RenderGUI(const engine::platform::Timer &timer, GrassCollection &grass_collection, WindEntity &wind_entity) {
    //ImGui::Text("Loading stats");
    //ImGui::Text("Scene loading time (ms): %lu", loading_clock_.GetDuration());
    //ImGui::Separator();
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
    bool wireframe = (render_device_.render_stats.previous_raster_state == engine::renderer::RenderDevice::RasterState::kNoCullWire);
    ImGui::Checkbox("Wireframe ", &wireframe); //ImGui::SameLine(150);
    if (wireframe) {
        render_device_.SetRasterState(engine::renderer::RenderDevice::RasterState::kNoCullWire);
    }
    else {
        render_device_.SetRasterState(engine::renderer::RenderDevice::RasterState::kNoCullSolid);
    }
    ImGui::Separator();
    ImGui::Text("Camera");
    ImGui::Text("Poition: x  %0.2f, y %0.2f, z %0.2f", camera_.GetPosition().x, camera_.GetPosition().y, camera_.GetPosition().z);
    float cam_speed = (float)camera_.GetSpeed();
    ImGui::SliderFloat("Speed unit/s ", &cam_speed, 1.4f, 5000.0f);
    camera_.SetSpeed(cam_speed);

    grass_renderer_.RenderGUI(grass_collection); 
    wind_renderer_.RenderGUI(wind_entity); 
}

void Renderer::Resize(const glm::ivec2 &window_size) {
    render_device_.SetViewport(glm::ivec2(0, 0), window_size);
    // Uprojection matrix
    uniform_data_.projection_from_view = glm::perspective(glm::radians(camera_.GetFOV()), window_size.x / (engine::tp::Real)window_size.y, z_near_, z_far_);
    uniform_buffer_.SubData(offsetof(UniformData, projection_from_view), sizeof(UniformData::projection_from_view), &uniform_data_.projection_from_view[0]);
}

// TODO: should not be here
void Renderer::MouseMove(const engine::platform::Input &input) {
    if (input.mouse.cursor_disabled) {
        camera_.ProcessMouseMovement(input.mouse.offset.x, input.mouse.offset.y);
    }
}

void Renderer::MouseScroll(const engine::platform::Input &input, const glm::ivec2 &window_size) {
    if (input.mouse.cursor_disabled) {
        camera_.ProcessMouseScroll(input.mouse.scroll_offset.y);
        // projection matrix
        uniform_data_.projection_from_view = glm::perspective(glm::radians(camera_.GetFOV()), window_size.x / (engine::tp::Real)window_size.y, z_near_, z_far_);
        uniform_buffer_.SubData(offsetof(UniformData, projection_from_view), sizeof(UniformData::projection_from_view), &uniform_data_.projection_from_view[0]);
    }
}

}; // grass