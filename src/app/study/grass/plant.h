#pragma once
#include <memory>
#include <engine/renderer/render_device.h>
#include <engine/core/types.h>
#include "types.h"

namespace app::study::grass {
class Plant {
public:
    Plant(engine::renderer::RenderDevice &render_device );
    void Render(engine::renderer::RenderDevice &render_device, engine::renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data,
               glm::tvec3<engine::tp::Real> position);
    void RenderGui();
private:
    void Init(engine::renderer::RenderDevice &render_device);
    std::vector<engine::renderer::gl::Buffer> vertex_buffers_;
    std::unique_ptr<engine::renderer::gl::VertexArray> vertex_array_;
    uint32_t vertex_count_;
};
};
