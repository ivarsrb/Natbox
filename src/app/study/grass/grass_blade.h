#pragma once
#include <memory>
#include <engine/renderer/render_device.h>
#include <engine/core/types.h>
#include "types.h"
#include "plant.h"

namespace app::study::grass {
class GrassBlade {
public:
    GrassBlade(engine::renderer::RenderDevice &render_device );
    void Update(engine::tp::Real dt);
    void Render(engine::renderer::RenderDevice &render_device, engine::renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data,
               engine::tp::Vec3 position);
    void RenderGui();
private:
    void Init(engine::renderer::RenderDevice &render_device);
    std::vector<engine::renderer::gl::Buffer> vertex_buffers_;
    std::unique_ptr<engine::renderer::gl::VertexArray> vertex_array_;
    std::vector<VertexType> vertices_;
    std::unique_ptr<Plant> blade_;
    engine::tp::Vec3 wind_;
};
};
