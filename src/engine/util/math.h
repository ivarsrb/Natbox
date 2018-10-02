#pragma once
#include <engine/core/types.h>

namespace engine::util::math {
// To represent spheric coordinate system
// http://mathworld.wolfram.com/SphericalCoordinates.html
class SphericVector {
public:
    // Which axis will be wrap-around (0 - 2pi)
    enum class AzimuthAxis {
        kZ = 0,
        kY
    };
    SphericVector(): radius(0), theta(0), phi(0), azimuth_axis_(AzimuthAxis::kY) {}
    // Vector in cartesian coordinate system is passed and transformed to spheric
    void InitFromCartesian(const tp::Vec3 &c_point, const AzimuthAxis azimuth = AzimuthAxis::kY);
    // Convert spherical vector to cartesian vector
    tp::Vec3 GetCartesian() const;

    tp::Real radius;
    // Azimuth (0 - 2pi)
    tp::Real theta;
    // Polar angle (inclination) (0 - pi)
    tp::Real phi;
private:
    // Wrap-around axis
    AzimuthAxis azimuth_axis_;
};
};