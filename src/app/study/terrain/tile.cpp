#include "tile.h"

using namespace engine;

namespace app::study::terrain {
Tile::Tile(engine::renderer::RenderDevice &render_device, const uint32_t tile_size) {
    Init(render_device, tile_size);
}

// Store vertex and index data for single terrain tile
// Vertices of a tile are always from 0 - 1 not depending on tile size
void Tile::Init(engine::renderer::RenderDevice &render_device, const uint32_t tile_size) {
    /*
    if (tile_size <= 0 || ((tile_size - 1) & (tile_size - 2)) != 0) {
        throw std::runtime_error("(Terrain tile) Wrong tile size");
    }
    */
    // Vertex
    std::vector<renderer::types::FPos> vertices;
    for (size_t y = 0; y < tile_size; y++) {
        for (size_t x = 0; x < tile_size; x++) {
            // Vertices go from 0 - 1
            vertices.push_back({ glm::vec3(x / (tile_size - 1.0), 0.0, y / (tile_size - 1.0)) });
        }
    }
    // Index
    std::vector<uint32_t> indices;
    uint32_t i = 0;
    // For triangles
    // Increase capacity to speed up pushback
    for (size_t y = 0; y < tile_size - 1; y++) {
        for (size_t x = 0; x < tile_size - 1; x++) {
            // triangle 1
            indices.emplace_back(i); // initial
            indices.emplace_back(i + 1); // to the right
            indices.emplace_back((i + 1) + tile_size); // to the right up

            // triangle 2
            indices.emplace_back(i); // initial
            indices.emplace_back((i + 1) + tile_size); // to the right up
            indices.emplace_back(i + tile_size); // up

            i++;
        }
        i++;
    }
    
    mesh_.SetData(render_device, vertices, indices);
}

void Tile::Render(engine::renderer::RenderDevice &render_device) {
    mesh_.Render(render_device);
}
};