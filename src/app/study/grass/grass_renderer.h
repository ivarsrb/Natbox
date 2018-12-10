#pragma once
#include <vector>
#include <optional>
#include <engine/renderer/gl/buffer.h>
#include <engine/renderer/gl/program.h>
#include <engine/renderer/gl/vertex_array.h>
#include <engine/renderer/gl/texture_2d.h>
#include "grass_collection.h"
#include "types.h"

namespace app::study::grass {
// Render grass blades using shared resources
class GrassRenderer {
public:
    GrassRenderer(const GrassCollection &collection);
    void Render(const GrassCollection &collection, engine::renderer::gl::Buffer &uniform_buffer, 
                UniformData &uniform_data);
    void RenderGUI(GrassCollection &collection);
private:
    // Locations of vertex array attributes in shader
    enum AttribLocation {
        kPosition = 0,
        kEdge = 1,
        kNormal = 2,
        kSide = 3,
        // Not an attribute but measurment for size
        kSizeMeasure
    };
    // Update whole vertex buffer 
    void UpdateVertexBuffer(const GrassCollection &collection);
    std::optional<engine::renderer::gl::Buffer> vertex_buffer_;
    std::optional<engine::renderer::gl::VertexArray> vertex_array_;
    std::vector<VertexTypeGrass> vertices_;
    std::optional<engine::renderer::gl::Program> pipeline_;
    std::optional<engine::renderer::gl::Texture2D> diffuse_map_;
};
}; // grass