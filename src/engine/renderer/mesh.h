#pragma once
#include <vector>
#include <memory>
#include "render_device.h"

namespace engine::renderer {
// Helper class of rendering of common meshes
// Takes in vertices, indices
// Creates buffers ands sets up attributes
// Mesh properties:
// - indexed
// - Position
class Mesh {
public:
    Mesh();
    // Create vertex and index buffers from given data, set up VAO
    void SetData(RenderDevice &render_device, const std::vector<types::FPos> &vertices, 
                 const std::vector<uint32_t> &indices);
    void Render(RenderDevice &render_device);
    void RenderStrip(RenderDevice &render_device);
private:
    // Using only one VBO becouse vector is needed to pass in VAO function
    std::vector<gl::Buffer> vertex_buffers_;
    std::unique_ptr<gl::Buffer> index_buffer_;
    std::unique_ptr<gl::VertexArray> vertex_array_;
    uint32_t index_count_ = 0;
};
};