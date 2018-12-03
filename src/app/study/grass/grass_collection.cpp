#include "grass_collection.h"
#include <engine/core/types.h>

namespace app::study::grass {
GrassCollection::GrassCollection() {
    //collection.emplace_back("grass_1.json", engine::tp::Vec3(0.0), 0.0, 1.0);
    collection.emplace_back("grass_3.json", engine::tp::Vec3(0.0), M_PI_2, 1.0);
}

uint32_t GrassCollection::GetTotalVertexCount() const {
    uint32_t vertex_count = 0;
    for (const auto &entity : collection) {
        vertex_count += entity.GetEdges().size();
    }
    return vertex_count;
}


void GrassCollection::Update(engine::tp::Real dt, const WindEntity &wind_entity) {
    for (auto &entity : collection) {
        entity.Update(dt, wind_entity);
    }
}
/*
void GrassCollection::Render(engine::renderer::RenderDevice &render_device) {

}*/

}; // grass