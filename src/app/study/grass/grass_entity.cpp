#include "grass_entity.h"
#include <engine/util/math.h>

namespace app::study::grass {
GrassEntity::GrassEntity(const std::string &model_file, engine::tp::Vec3 world_position, 
               engine::tp::Real orientation, engine::tp::Real scale): grass_model_(model_file),
                                                                      grass_physics_(grass_model_.GetEdges(), grass_model_.GetMass(), grass_model_.GetWidth()),
                                                                      world_position_(world_position),
                                                                      orientation_(orientation),
                                                                      scale_(scale) 
{

}

void GrassEntity::Update(engine::tp::Real dt, const WindEntity &wind_entity) {
    auto wind_vec = wind_entity.GetVectorFromPosition(world_position_);
    // Adjust wind vector accordin to rotation of entity
    wind_vec = engine::util::math::RotateSafe(wind_vec, -orientation_, engine::util::math::kGroundNormal);
    grass_physics_.Update(dt, grass_model_.GetEdges(), grass_model_.GetStaticEdges(), wind_vec);
    grass_model_.UpdateOrientationVectors();
}

void GrassEntity::Reset() {
    grass_model_.Reset();
    grass_physics_.Reset();
}

}; // grass