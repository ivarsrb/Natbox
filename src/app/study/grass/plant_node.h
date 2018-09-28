#pragma once
//#include <vector>
//#include "types.h"
#include <engine/core/types.h>
//#include <engine/util/math.h>

namespace app::study::grass {
/*
// Each node stores it's node's vector in spherical coordinates
class PlantNode {
public:
    // Node vector is passed in cartesian coordinates and transformed into spherical for calculations of rotation
    PlantNode(const engine::tp::Pos3D &c_vector);
    //PlantNode(const engine::tp::Pos3D &origin, const engine::util::Math::SphericCoords &polar_coord);
    //engine::tp::Pos3D Endpoint() const;
    // Calculate  new vector orientation from given forces
    void Update(engine::tp::Real dt, const engine::tp::Force3D &wind);
    // Get cartesian vector from current spherical node vector
    engine::tp::Pos3D CartesianVector() const;
    //void SetOrigin(const engine::tp::Pos3D &origin) {origin_ = origin;}
    //engine::tp::Pos3D Origin() const {return origin_;}
    // For debugging
    void Print() const;
private:
    // Current node vector orientation when animating
    engine::util::Math::SphericCoords s_vector_;
    // Node vector basic orientation (how model actually is without animation)
    engine::util::Math::SphericCoords static_s_vector_;
    engine::tp::Force3D angular_velocity_;

};
*/
class PlantNode {
public:
    PlantNode(const engine::tp::Vec3 &vector);
    void Update(engine::tp::Real dt, const engine::tp::Vec3 &wind);
    engine::tp::Vec3 Vector() const;
private:
    // During animation
    engine::tp::Vec3 current_vector_;
    engine::tp::Vec3 current_normal_;
    engine::tp::Vec3 current_side_;
    // Before animation
    engine::tp::Vec3 static_vector_;
    engine::tp::Vec3 static_normal_;
    engine::tp::Vec3 static_side_;

    engine::tp::Vec3 angular_velocity_;
};

};