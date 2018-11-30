#pragma once
#include <engine/core/types.h>
#include <engine/util/math.h>

namespace app::study::grass {
class WindEntity {
public:
    WindEntity();
    void Reset();
    void Stop();
    void SetSpeed(engine::tp::Real speed);
    // Rotation angle theta of wind vector in spherical coordinates
    void SetRotation(engine::tp::Real angle);
    engine::tp::Vec3 GetVectorFromPosition(const engine::tp::Vec3 &pos) const;
    engine::util::math::SphericVector GetSphericVectorFromPosition(const engine::tp::Vec3 &pos) const;
    bool IsBlowing();
private:
    // Radius is strength of wind, theta rotates it around y axis
    engine::util::math::SphericVector s_vector_;
};
}; // grass