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
    assert(c_point.x != 0 || azimuth_value != 0);
    radius = glm::length(c_point);
    theta = atan2(azimuth_value, c_point.x);
    // Since atan2 return from 0 to and from -pi to 0, transform so that angle goes from 0 to 2*pi
    theta = theta < 0 ? theta + 2*M_PI : theta;
    assert(radius > 0);
    phi = acos(inclination_value / radius);
}

// Convert spherical vector to cartesian vector
tp::Vec3 SphericVector::GetCartesian() const {
    tp::Vec3 c_point;
    // Value whoch rotates around azimuth axis (except x, because it's always rotates around azimuth)
    tp::Real azimuth_value = 0;
    // Value which will determine inclination (phi)
    tp::Real inclination_value = 0;

    c_point.x = radius * cos(theta) * sin(phi);
    azimuth_value = radius * sin(theta) * sin(phi);
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

/*
// Convert point certesian coordinates to spherical coordinates (get radius, theta and phi) 
Math::SphericCoords Math::CartesianToSpheric(const tp::Vec3 &c_point) {
    assert(c_point.x != 0 || c_point.y != 0);
    SphericCoords spheric;
    spheric.radius = glm::length(c_point);
    spheric.theta = atan2(c_point.y, c_point.x);
    // Since atan2 return from 0 to and from -pi to 0, transform so that angle goes from 0 to 2*pi
    spheric.theta = spheric.theta < 0 ? spheric.theta + 2*M_PI : spheric.theta;
    assert(spheric.radius > 0);
    spheric.phi = acos(c_point.z / spheric.radius);
    return spheric;
}

// Convert spherical coordinates to certesian point
tp::Vec3 Math::SphericToCertesian(const SphericCoords &s_coord) {
    tp::Vec3 c_point;
    c_point.x = s_coord.radius * cos(s_coord.theta) * sin(s_coord.phi);
    c_point.y = s_coord.radius * sin(s_coord.theta) * sin(s_coord.phi);
    c_point.z = s_coord.radius * cos(s_coord.phi);;
    return c_point;
}
*/

};