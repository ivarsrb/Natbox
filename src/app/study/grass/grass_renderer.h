#pragma once
#include <vector>
#include <optional>
#include <engine/renderer/render_device.h>
#include "grass_collection.h"
#include "types.h"

namespace app::study::grass {
// Render grass blades using shared resources
class GrassRenderer {
public:
    GrassRenderer(engine::renderer::RenderDevice &render_device, const GrassCollection &collection);
    void Render(engine::renderer::RenderDevice &render_device, const GrassCollection &collection, engine::renderer::gl::Buffer &uniform_buffer, 
                UniformData &uniform_data);
    void RenderGUI(GrassCollection &collection);
private:
    std::optional<engine::renderer::gl::Buffer> vertex_buffer_;
    std::optional<engine::renderer::gl::VertexArray> vertex_array_;
    std::vector<VertexType> vertices_;
    std::optional<engine::renderer::gl::Program> pipeline_;
    std::optional<engine::renderer::gl::Texture2D> diffuse_map_;
};
}; // grass