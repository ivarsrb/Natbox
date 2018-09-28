#include "plant_node.h"
#include <iostream>
#include <glm/glm.hpp>
//#include <engine/util/log.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

using namespace engine;

namespace app::study::grass {
/*
PlantNode::PlantNode(const tp::Pos3D &c_vector) {
    s_vector_ = static_s_vector_ = util::Math::CartesianToSpheric(c_vector);
    angular_velocity_ = tp::Force3D(0.0,0.0,0.0);
}

// Calculate  new vector orientation from given forces
void PlantNode::Update(tp::Real dt, const tp::Force3D &wind) {
    tp::Pos3D c_vector = CartesianVector();
    tp::Pos3D c_vector_normal = glm::cross(c_vector, tp::Pos3D(0.0,0.0,1.0));
    //std :: cout << c_vector_normal.x << " " << c_vector_normal.y << " " << c_vector_normal.z << std::endl;
    // W = S * omega * (v_ * En) * En;
    tp::Force3D deform_force = 1.0 * glm::dot(wind, c_vector_normal) * c_vector_normal;
    // R = k * delta_Fi * delta_s_
    tp::Pos3D c_static_vector = util::Math::SphericToCertesian(static_s_vector_) ;
    tp::Force3D restore_force = //0.1 * (static_s_vector_.theta - s_vector_.theta) * glm::normalize(c_static_vector- c_vector); 
    // F = W + R
    tp::Force3D totoal_force = deform_force;// + restore_force;

    // I = m*r^2
    tp::Real inertia = 0.1 * s_vector_.radius * s_vector_.radius;
    // N = Ee x F
    tp::Force3D torque = glm::cross(c_vector, totoal_force);
    // a_ = N / I
    tp::Force3D angular_accel = torque / inertia;
    // w_ = w_ + a_*dt
    angular_velocity_ += angular_accel * dt;
    // d_theta = w_ * dt
    tp::Force3D dtheta = angular_velocity_  * dt;

    c_vector = glm::rotate(c_vector,glm::length(dtheta), glm::normalize(dtheta) );
    s_vector_  = util::Math::CartesianToSpheric(c_vector);
    //s_vector_.theta += dtheta.z;
    //std :: cout << "---------------------\n";
}

// Get cartesian vector from current spherical node vector
engine::tp::Pos3D PlantNode::CartesianVector() const {
    return util::Math::SphericToCertesian(s_vector_);
}

// For debugging
void PlantNode::Print() const {
    util::Log::Write("Plant node - radius: '",s_vector_.radius,"', theta: '",glm::degrees(s_vector_.theta),"', phit: '",glm::degrees(s_vector_.phi), "'" );
}
*/

PlantNode::PlantNode(const tp::Vec3 &vector) {
    current_vector_ = static_vector_ = vector;
    current_side_ = static_side_ = tp::Vec3(0.0,0.0,1.0);
    current_normal_ = static_normal_ = glm::cross(current_vector_, current_side_);
    angular_velocity_ = tp::Vec3(0.0,0.0,0.0);
}

tp::Vec3 PlantNode::Vector() const {
    return current_vector_;
}

void PlantNode::Update(engine::tp::Real dt, const engine::tp::Vec3 &wind) {
    current_normal_ = glm::cross(current_vector_, current_side_);
    // W = S * omega * (v_ * En) * En;
    tp::Vec3 deform_force = 1.0 * glm::dot(wind, current_normal_) * current_normal_;
    //std::cout << glm::to_string(deform_force) << std::endl;
    // R = k * delta_Fi * delta_s_f
    tp::Vec3 restore_force(0.0, 0.0, 0.0);
    if (static_vector_ != current_vector_) {
        restore_force = 1.0 * glm::angle(glm::normalize(static_vector_), glm::normalize(current_vector_)) * glm::normalize(static_vector_ - current_vector_); 
        //std::cout << glm::to_string(restore_force) << std::endl;
    }
    // F = W + R
    tp::Vec3 totoal_force = deform_force + restore_force;
    // I = m*r^2
    tp::Real inertia = 0.1 * glm::length(current_vector_) * glm::length(current_vector_);
    // N = Ee x F
    tp::Vec3 torque = glm::cross(current_vector_, totoal_force);
    // a_ = N / I
    tp::Vec3 angular_accel = torque / inertia;
    // w_ = w_ + a_*dt
    angular_velocity_ += angular_accel * dt;
    // d_theta = w_ * dt
    tp::Vec3 dtheta = angular_velocity_ * dt;

    if(glm::length(dtheta) != 0) {
        // angular velocity rotates a vector around it normalized axis by magnitude of angular velocity vector
        current_vector_ = glm::rotate(current_vector_, glm::length(dtheta), glm::normalize(dtheta));
    }
}
};
