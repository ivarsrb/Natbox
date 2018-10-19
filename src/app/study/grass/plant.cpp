#include "plant.h"
#include <iostream>
#include <engine/util/math.h>
#include <engine/util/log.h>
#define GLM_ENABLE_EXPERIMENTAL 
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace engine;

namespace app::study::grass {
// NOTE: First point of edges array should always be a root of a plant and store 4 edges
Plant::Plant(const std::vector<tp::Vec3> &edges): static_edges_(edges) {
    assert(static_edges_.size() == 4);
    growth_static_vec_ = static_edges_.at(3) - static_edges_.at(0);
    GetNormalSideVectors(growth_static_vec_,normal_static_vec_,side_static_vec_);
    //util::Log::Write("Plant growth vec: ",glm::to_string(growth_static_vec_), ", normal ", glm::to_string(normal_static_vec_) ,", side ", glm::to_string(side_static_vec_));
    Reset();
    // Stiffness should come with grass edges
    stiffness_.push_back(std::numeric_limits<float>::infinity());
    stiffness_.push_back(5);
    stiffness_.push_back(3);
    stiffness_.push_back(2.0);
    // mass of a blade
    mass_ = 0.001;
}

// Set all edges and nodes to it's original shape
void Plant::Reset() {
    util::Log::Write("Reset plant");
    edges_ = static_edges_;
    growth_vec_ = growth_static_vec_;
    normal_vec_ = normal_static_vec_;
    side_vec_ = side_static_vec_;
    angular_velocity_s_ = tp::Vec3(0.0, 0.0, 0.0);
    angular_velocity_b_ = tp::Vec3(0.0, 0.0, 0.0);

   // angular_displacement_ = 0;
    //nodes_.clear();
    //CreateNodesFromEdges(edges_);
}

// Calculate vector for each section and create node given this vector in cartesian coordinates
// NOTE: order of nodes is important, it should match order of edges
/*
void Plant::CreateNodesFromEdges(const std::vector<tp::Vec3> &edges) {

    for (auto edge_it = edges.cbegin(); edge_it != std::prev(edges.cend()); ++edge_it) {
        auto next_edge_it = std::next(edge_it);
        tp::Vec3 node_vec = *next_edge_it - *edge_it;
        nodes_.emplace_back(node_vec);
    }
}
*/

void Plant::FillVertices(std::vector<VertexType> &vertices, uint32_t start_index) const {
    for (const auto& edge : edges_) {
        vertices.at(start_index++).position = edge;
    }
}

// Perform swing action and return swinging delta theta (vector is axis, length is angle)
tp::Vec3 Plant::Swing(tp::Real dt, const tp::Vec3 &wind) {
    // Growth vectors projected on the ground plane (x, z)
    //tp::Vec3 growth_proj_vec = tp::Vec3(growth_vec_.x, 0.0, growth_vec_.z);
    //tp::Vec3 growth_proj_static_vec = tp::Vec3(growth_static_vec_.x, 0.0, growth_static_vec_.z);
    tp::Vec3 growth_proj_vec = util::math::GetXZProj(growth_vec_);
    tp::Vec3 growth_proj_static_vec = util::math::GetXZProj(growth_static_vec_);
    // Wind force
    tp::Vec3 wind_force_swing_ = 1.0 * glm::dot(wind, side_vec_) * side_vec_;
    //util::Log::Write("Wind force swing: ",glm::to_string(wind_force_swing_));
    // Restoration force
    tp::Vec3 restore_force_swing_(0,0,0);
    if (growth_proj_static_vec != growth_proj_vec) {
        restore_force_swing_ = stiffness_.back() * glm::angle(glm::normalize(growth_proj_static_vec), glm::normalize(growth_proj_vec))/* abs(angular_displacement_)*/* glm::normalize(growth_proj_static_vec - growth_proj_vec);                          
        //util::Log::Write("Restoration force swing: ", glm::to_string(restore_force_swing_));
    }
    // To damp down swinging when wind stops
    //angular_velocity_s_ += -0.9 * dt * angular_velocity_s_;
    //dampening_force_swing = -0.1 * restore_force_swing_;

    //util::Log::Write("damps  ",  glm::to_string(dampening_force_swing ));
    // Full force acting on plant
    tp::Vec3 full_force_swing_ = wind_force_swing_ + restore_force_swing_ ;

    tp::Real inertia = mass_ * glm::length(growth_vec_) * glm::length(growth_vec_);
    tp::Vec3 torque = glm::cross(growth_proj_vec, full_force_swing_);
    tp::Vec3 angular_accel = torque / inertia;
    angular_velocity_s_ += angular_accel * dt;
    //util::Log::Write("force-------- ",  glm::to_string(angular_velocity_) );
    tp::Vec3 dtheta = angular_velocity_s_ * dt;
    //angular_displacement_ += glm::length(dtheta)*(dtheta.y < 0.0 ? -1.0 : 1.0);
    //util::Log::Write("force-------- ",  glm::to_string(dtheta) );
    //util::Log::Write("ang dis  ",  angular_displacement_ );

    return dtheta;
}

// Perform bend action
tp::Vec3 Plant::Bend(tp::Real dt, const tp::Vec3 &wind) {
    // Wind force
    tp::Vec3 wind_force_ = 1.0 * glm::dot(wind, normal_vec_) * normal_vec_;
    // Restoration force
    tp::Vec3 restore_force_(0,0,0);
    if (growth_static_vec_ != growth_vec_) {
        restore_force_ = stiffness_.back() * glm::angle(glm::normalize(growth_static_vec_), glm::normalize(growth_vec_)) * glm::normalize(growth_static_vec_ - growth_vec_);                          
    }
    // To damp down swinging when wind stops
    //angular_velocity_b_ += -0.9 * dt * angular_velocity_b_;
    // Full force acting on plant
    tp::Vec3 full_force_ = wind_force_ + restore_force_ ;

    tp::Real inertia = mass_ * glm::length(growth_vec_) * glm::length(growth_vec_);
    tp::Vec3 torque = glm::cross(growth_vec_, full_force_);
    tp::Vec3 angular_accel = torque / inertia;
    angular_velocity_b_ += angular_accel * dt;
    
    tp::Vec3 dtheta = angular_velocity_b_ * dt;
    //util::Log::Write("torque  ",  glm::to_string(torque));
    //util::Log::Write("accel----- -------- ",  glm::to_string(angular_accel), "  ", dt );
    //util::Log::Write("vel ----- ",  glm::to_string(angular_velocity_b_));
    //util::Log::Write("dtheta ----- ",  glm::to_string(dtheta));
    //angular_displacement_ += glm::length(dtheta)*(dtheta.y < 0.0 ? -1.0 : 1.0);
    //util::Log::Write("force-------- ",  glm::to_string(dtheta) );
    //util::Log::Write("Ang accel  ",  glm::to_string(angular_accel));

    return dtheta;
}

// Animate nodes and calculate new edge points of a plant one by one starting at first after the root
void Plant::Update(tp::Real dt, const tp::Vec3 &wind) {
    tp::Vec3 dtheta_b = Bend(dt, wind);
    uint32_t counter = 0;
    tp::Real tip_dtheta = glm::length(dtheta_b);
    tp::Vec3 rot_axis = glm::normalize(dtheta_b);
    for (auto& edge : edges_) {
        if (tip_dtheta != 0.0 && counter > 0) {
            //tp::Real current_dtheta = (stiffness_.back() / stiffness_.at(counter)) * tip_dtheta;
            edge = glm::rotate(edge, tip_dtheta, rot_axis);
        }
        ++counter;
    }

    /*
    tp::Vec3 dtheta_s = Swing(dt, wind);
    // Rotate edges
    uint32_t counter = 0;
    tp::Real tip_dtheta = glm::length(dtheta_s);
    tp::Vec3 rot_axis = glm::normalize(dtheta_s);
    for (auto& edge : edges_) {
        if (tip_dtheta != 0.0 && counter > 0) {
            tp::Real current_dtheta = (stiffness_.back() / stiffness_.at(counter)) * tip_dtheta;
            edge = glm::rotate(edge, current_dtheta, rot_axis);
        }
        ++counter;
    }*/

    growth_vec_ = edges_.at(3) - edges_.at(0);
    GetNormalSideVectors(growth_vec_,normal_vec_,side_vec_);
/*
    // Since first edge of a plant is root, it always remains the same and we dont update it
    uint32_t edge_num = 1;
    for (auto& node : nodes_) {
        node.Update(dt, wind);
        edges_.at(edge_num) = edges_.at(edge_num - 1) + node.Vector();
        edge_num++;
    }
*/
}

// Get normal and normalized side vectors given growth vector
void Plant::GetNormalSideVectors(const engine::tp::Vec3 &growth, engine::tp::Vec3 &normal, engine::tp::Vec3 &side) const {
    side = glm::normalize(glm::cross(growth, util::math::kGroundNormal));
    normal = glm::normalize(glm::cross(side, growth));
}

};