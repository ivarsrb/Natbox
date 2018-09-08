#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <engine/renderer/render_device.h>
#include <engine/renderer/mesh.h>

// Physical representation of a tile
namespace app::study::terrain {
class Tile {
public:
    Tile(engine::renderer::RenderDevice &render_device, const uint32_t tile_size);
    void Render(engine::renderer::RenderDevice &render_device);
private:
    void Init(engine::renderer::RenderDevice &render_device, const uint32_t tile_size);
    engine::renderer::Mesh mesh_;
};
};