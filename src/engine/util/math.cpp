#include "math.h"
#include <cmath>
#include <cassert>
#include <iostream>
#include <glm/glm.hpp>
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/vector_angle.hpp>
//#include <glm/gtx/polar_coordinates.hpp>
//#include <glm/gtx/string_cast.hpp>

namespace engine::util {
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

};