#pragma once
#include <engine/core/types.h>

namespace engine::util::math {
// This tells us that ground plane is x-z plane and normal goes up
// TODO this constant should belong somehere else
constexpr tp::Vec3 kGroundNormal(0.0, 1.0, 0.0);

// To represent spheric coordinate system for RIGHT-handed coordinate system
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
    // In case Y axis is up (AzimuthAxis::kY) angle goes from +Z to +X direction (counter clockwise)
    tp::Real theta;
    // Polar angle (inclination) (0 - pi) 
    // In case Y axis is up (AzimuthAxis::kY) angle goes from +Y up being 0 to -Y being PI 
    tp::Real phi;
private:
    // Wrap-around axis
    AzimuthAxis azimuth_axis_;
};

// Utility functions
// Use glm function to normalize, but return same vector vector if given vector is zero
tp::Vec3 NormalizeSafe(const tp::Vec3 &vector);
// If axis is 0 return the same point, without this rotation would be - nan
tp::Vec3 RotateSafe(const tp::Vec3 &point, const tp::Real angle, const tp::Vec3 &axis);
// Find given vectors projections vector onto XZ plane
tp::Vec3 GetXZProj(const tp::Vec3 &vector);

};