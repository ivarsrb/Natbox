#include "math.h"
#include <cmath>
#include <cassert>
#include <iostream>
#include <glm/glm.hpp>
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/vector_angle.hpp>
//#include <glm/gtx/polar_coordinates.hpp>
//#include <glm/gtx/string_cast.hpp>

namespace engine::util::math {

// Vector in cartesian coordinate system is passed and transformed to spheric
void SphericVector::InitFromCartesian(const tp::Vec3 &c_point, const AzimuthAxis azimuth) {
    azimuth_axis_ = azimuth;
    // Value whoch rotates around azimuth axis (except x, because it's always rotates around azimuth)
    tp::Real azimuth_value = 0;
    // Value which will determine inclination (phi)
    tp::Real inclination_value = 0;
    switch (azimuth_axis_) {
        case AzimuthAxis::kY:
            azimuth_value = c_point.z;
            inclination_value = c_point.y;
            break;
        case AzimuthAxis::kZ:
            azimuth_value = c_point.y;
            inclination_value = c_point.z;
            break;
        default:
            break;
    }
    radius = glm::length(c_point);
    if (radius > 0) {
        //assert(c_point.x != 0 || azimuth_value != 0);
        theta = atan2(c_point.x, azimuth_value);
        // Since atan2 return from 0 to and from -pi to 0, transform so that angle goes from 0 to 2*pi
        theta = theta < 0 ? theta + 2*M_PI : theta;
        //assert(radius > 0);
        phi = acos(inclination_value / radius);
    } else {
        theta = 0.0;
        phi = 0.0;
    }
}

// Convert spherical vector to cartesian vector
tp::Vec3 SphericVector::GetCartesian() const {
    tp::Vec3 c_point;
    // Value whoch rotates around azimuth axis (except x, because it's always rotates around azimuth)
    tp::Real azimuth_value = 0;
    // Value which will determine inclination (phi)
    tp::Real inclination_value = 0;

    c_point.x = radius * sin(theta) * sin(phi);
    azimuth_value = radius * cos(theta) * sin(phi);
    inclination_value = radius * cos(phi);;
    
    switch (azimuth_axis_) {
        case AzimuthAxis::kY:
            c_point.z = azimuth_value;
            c_point.y = inclination_value;
            break;
        case AzimuthAxis::kZ:
            c_point.y = azimuth_value;
            c_point.z = inclination_value;
            break;
        default:
            break;
    }
    
    return c_point;
}

// Utility functions
// Use glm function to normalize, but return 0 vector if given vector is zero
tp::Vec3 NormalizeSafe(const tp::Vec3 &vector) {
    return (glm::length(vector) ? glm::normalize(vector) : vector);
}


// Find given vectors projections vector onto XZ plane
tp::Vec3 GetXZProj(const tp::Vec3 &vector) {
    return tp::Vec3(vector.x, 0.0, vector.z);
}

};