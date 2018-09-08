#pragma once
#include <memory>
#include <engine/renderer/render_device.h>
#include <engine/core/types.h>
//#include <engine/core/camera.h>
#include "types.h"
#include "tile.h"

namespace app::study::terrain {
class Terrain {
public:
    struct TerrainDescr {
        // Number of posts (vertices) per diension in root tile
        uint32_t resolution;
        // horizontal extent of a terrain
        // Tile is square, so its equal in both direction 
        engine::tp::Real extent;
        // By how much to multiply height values to go from local to world
        engine::tp::Real vertical_scale;
        std::string heightfield_source_file;
    };

    Terrain(engine::renderer::RenderDevice &render_device );
    void Render(engine::renderer::RenderDevice &render_device,  engine::renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data);
    void RenderGui();
private:
    void Init(engine::renderer::RenderDevice &render_device);
    void GetHeightfield(engine::renderer::RenderDevice &render_device, const std::string file_name);
    TerrainDescr descr_;
    // Physical representation of single tile (or terrain if no LOD is used)
    std::unique_ptr<Tile> tile_;
    // Texture to read height from
    std::unique_ptr<engine::renderer::gl::Texture2D> heighfield_;
    glm::tmat4x4<engine::tp::Real> world_from_local_;
    std::unique_ptr<engine::renderer::gl::Texture2D> diffuse_map_1_;
};
};