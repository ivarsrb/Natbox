#include "wind_entity.h"
#include <engine/util/log.h>

namespace app::study::grass {
WindEntity::WindEntity() {
    Reset();
}

void WindEntity::Reset() {
    // Initial direction and strenth in cartesian coordinates system
    engine::tp::Vec3 c_initial_vector_ = engine::tp::Vec3(0.0, 0.0, 1.0);
    // Transform to spherical
    s_vector_.InitFromCartesian(c_initial_vector_); 
}

void WindEntity::Stop() {
    SetSpeed(0);
}

// In spherical coordinates speed is represented by radius 
void WindEntity::SetSpeed(engine::tp::Real speed) {
    if (speed < 0) {
        speed = 0.0;
        engine::util::Log::Write2("Wind speed is negative; set to 0");
    }    
    s_vector_.radius = speed;
}

// Rotation angle theta of wind vector in spherical coordinates
void WindEntity::SetRotation(engine::tp::Real angle) {
    s_vector_.theta = angle;
}

// Get wind vector in given position
engine::tp::Vec3 WindEntity::GetVectorFromPosition(const engine::tp::Vec3 &pos) const {
    // TODO add position sampling here
    return s_vector_.GetCartesian();
}

engine::util::math::SphericVector WindEntity::GetSphericVectorFromPosition(const engine::tp::Vec3 &pos) const {
    return s_vector_;
}

bool WindEntity::IsBlowing() {
    return (bool)s_vector_.radius;
}
}; // grass