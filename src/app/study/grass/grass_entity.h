#pragma once
#include <string>
#include <engine/core/types.h>
#include "grass_model.h"
#include "grass_physics.h"
#include "wind_entity.h"

namespace app::study::grass {
class GrassEntity {
public:
    GrassEntity(const std::string &model_file, engine::tp::Vec3 world_position, 
               engine::tp::Real orientation, engine::tp::Real scale);
    void Update(engine::tp::Real dt, const WindEntity &wind_entity);
    void Reset();
    const std::vector<GrassModel::Edge>& GetEdges() const {return grass_model_.GetEdges();};
    const engine::tp::Vec3 GetWorldPosition() const { return world_position_; }
    const engine::tp::Real GetOrientation() const { return orientation_; }
private:
    GrassModel grass_model_;
    GrassPhysics grass_physics_;
    // These property NEVER changes during simulation
    engine::tp::Vec3 world_position_;
    // Angle of orientation about ground normal 
    engine::tp::Real orientation_;
    engine::tp::Real scale_;
};
}; // grass