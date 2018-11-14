#pragma once
#include <vector>
#include <optional>
#include <engine/core/types.h>
#include <engine/renderer/render_device.h>
#include "entity.h"
#include "wind.h"

namespace app::study::grass {
// Collection and management of grass entities
// Stores texture and shader data
class Collection {
public:
    Collection(engine::renderer::RenderDevice &render_device);
    void Init(engine::renderer::RenderDevice &render_device);
    void Update(engine::tp::Real dt, const Wind &wind);
    void Render(engine::renderer::RenderDevice &render_device, engine::renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data);
    void RenderGui();
private:
    std::vector<Entity> entities_;
    std::optional<engine::renderer::gl::Program> pipeline_;
};
}; // grass