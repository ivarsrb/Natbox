#pragma once 
#include <engine/util/math.h>

namespace app::study::grass {
class Wind {
public:
    Wind();
    void Reset();
    void Stop();
    void Update(engine::tp::Real dt);
    void SetSpeed(engine::tp::Real speed);
    // Rotation angle theta of wind vector in spherical coordinates
    void SetRotation(engine::tp::Real angle);
    void RenderGui();
    engine::tp::Vec3 CartesianVector() const;
private:
    // Radius is strength of wind, theta rotates it around y axis
    engine::util::math::SphericVector s_vector_;
};
};