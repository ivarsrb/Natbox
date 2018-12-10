#pragma once
#include <vector>
#include <optional>
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
    void SetData(const std::vector<types::FPos> &vertices, 
                 const std::vector<uint32_t> &indices);
    void Render();
    void RenderStrip();
private:
    // Using only one VBO becouse vector is needed to pass in VAO function
    std::optional<gl::Buffer> vertex_buffer_;
    std::optional<gl::Buffer> index_buffer_;
    std::optional<gl::VertexArray> vertex_array_;
    uint32_t index_count_ = 0;
};
};