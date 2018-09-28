#pragma once
#include <engine/core/types.h>

namespace engine::util {
class Math {
public:
    // To represent spheric coordinate system
    // http://mathworld.wolfram.com/SphericalCoordinates.html
    struct SphericCoords {
        tp::Real radius = 0.0;
        // Wrap-around z axis (0-2*pi) (azimuth)
        tp::Real theta = 0.0;
        // 0-pi , goes from z-axis (polar angle)
        tp::Real phi = 0.0;
    };
    // Convert point certesian coordinates to spherical coordinates (get radius, theta and phi) 
    static SphericCoords CartesianToSpheric(const tp::Vec3 &c_point);
    // Convert spherical coordinates to certesian point
    static tp::Vec3 SphericToCertesian(const SphericCoords &s_coord);
};
};