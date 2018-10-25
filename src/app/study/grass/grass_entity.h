#pragma once
#include <vector>
#include <optional>
#include <engine/core/types.h>
#include <engine/renderer/render_device.h>
#include "grass_physics.h"
#include "wind.h"

namespace app::study::grass {
// Single grass object update and rendering
class GrassEntity {
public:
    // Types of available grass
    enum class Type {
        kGrass1 = 0,
        kGrass2
    };
    // Per grass entity data
    struct Properties {
        // Type of a grass entity
        Type type;
        // Entities position in thw wrold
        engine::tp::Vec3 world_position;
        // Anti-clockwise rotation of object about ground normal axis
        engine::tp::Real rotation;
        // Entity scale from models original scale
        engine::tp::Real scale;
    };
    GrassEntity(engine::renderer::RenderDevice &render_device, const Properties &properties);
    void Init(engine::renderer::RenderDevice &render_device);
    void Update(engine::tp::Real dt, const Wind &wind);
    void Render(engine::renderer::RenderDevice &render_device, engine::renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data);
    void RenderGui();
    void EdgesToVertices(const std::vector<GrassPhysics::EdgeProperties> &edges, uint32_t start_index);
private:
    Properties properties_;
    // Rendering
    std::vector<engine::renderer::gl::Buffer> vertex_buffers_;
    std::optional<engine::renderer::gl::VertexArray> vertex_array_;
    std::vector<VertexType> vertices_;
    // Simulate animation of grass edges in dynamic wind force and return new calculated edges
    std::optional<GrassPhysics> physics_;
};
}; // grass