#pragma once
#include <vector>
//#include <engine/renderer/render_device.h>
#include "grass_entity.h"
#include "wind_entity.h"

namespace app::study::grass {
class GrassCollection {
public:
    GrassCollection();
    void Update(engine::tp::Real dt, const WindEntity &wind_entity);
    //void Render(engine::renderer::RenderDevice &render_device);
    uint32_t GetTotalVertexCount() const;
    std::vector<GrassEntity> collection;    
};
}; // grass