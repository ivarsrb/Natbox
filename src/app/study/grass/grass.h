#pragma once
#include <vector>
#include <engine/core/types.h>
#include <engine/renderer/render_device.h>
#include "grass_entity.h"
#include "wind.h"

namespace app::study::grass {
// Collection and management of grass entities
// Stores texture and shader data
class Grass {
public:
    Grass(engine::renderer::RenderDevice &render_device);
    void Init(engine::renderer::RenderDevice &render_device);
    void Update(engine::tp::Real dt, const Wind &wind);
    void Render(engine::renderer::RenderDevice &render_device, engine::renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data);
    void RenderGui();
private:
    std::vector<GrassEntity> entities_;
    std::unique_ptr<engine::renderer::gl::Program> pipeline_;
};
}; // grass