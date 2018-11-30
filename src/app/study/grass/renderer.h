#pragma once
#include <glm/glm.hpp>
#include <engine/renderer/render_device.h>
#include <engine/core/types.h>
#include <engine/core/camera.h>
#include <engine/platform/input.h>
#include <engine/platform/timer.h>
#include "types.h"
#include "grass_renderer.h"
#include "grass_collection.h"
#include "wind_renderer.h"
#include "wind_entity.h"

namespace app::study::grass {
class Renderer {
public:
    Renderer(const glm::ivec2 &window_size, const GrassCollection &grass_collection);
    void Update(const engine::tp::Real dt, const engine::platform::Input &input);
    void Render(const GrassCollection &grass_collection, const WindEntity &wind_entity);
    void RenderGUI(const engine::platform::Timer &timer, GrassCollection &grass_collection, WindEntity &wind_entity);
    void Resize(const glm::ivec2 &window_size);
    void MouseMove(const engine::platform::Input &input); 
    void MouseScroll(const engine::platform::Input &input, const glm::ivec2 &window_size);
private: 

    engine::core::Camera camera_;
    engine::renderer::RenderDevice render_device_;
    engine::tp::Real z_near_;
    engine::tp::Real z_far_;
    UniformData uniform_data_;
    engine::renderer::gl::Buffer uniform_buffer_;
    // Component rendering
    GrassRenderer grass_renderer_;
    WindRenderer wind_renderer_;
};
}; // grass