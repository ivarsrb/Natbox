#include "grass_physics.h"
#include <cmath>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL 
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <engine/util/math.h>
#include <engine/util/log.h>

using namespace engine;

namespace app::study::grass {
GrassPhysics::GrassPhysics(const std::vector<EdgeProperties> &edges): edges_static_(edges) {
    // By definition edge count is always 4
    assert(edges_static_.size() == 4);
    // Fill up properties
    CalcFixedProperties(props_static_, edges_static_);
    CalcSimVectors(props_static_, edges_static_);
    Reset();
}

// Reset simulation to it's initial state
void GrassPhysics::Reset() {
    // Edges
    edges_ = edges_static_;
    // Properties
    props_ = props_static_;
    // Motions
    swing_.angular_velocity = tp::Vec3(0.0);
    swing_.displacement_ang = 0;
    bend_.angular_velocity = tp::Vec3(0.0);
    bend_.displacement_ang = 0;
}

// Return currently modified edges
const std::vector<GrassPhysics::EdgeProperties>& GrassPhysics::CurrentEdges() const {
    return edges_;
}

void GrassPhysics::Update(tp::Real dt, const tp::Vec3 &wind_vec) {
    Rotation rotation;
    
    // Bend
    CalcSimVectors(props_, edges_);
    rotation = BendMotion(dt, wind_vec);
    if (rotation.angle) {
        for (uint32_t i = 1; i < edges_static_.size(); ++i) {  
            edges_.at(i).position = glm::rotate(edges_static_.at(i).position, rotation.angle, rotation.axis);
        }
    }
    
    // Swing
    CalcSimVectors(props_, edges_);
    rotation = SwingMotion(dt, wind_vec);
    if (rotation.angle) {
        for (uint32_t i = 1; i < edges_static_.size(); ++i) {  
            edges_.at(i).position = glm::rotate(edges_static_.at(i).position, rotation.angle, rotation.axis);
        }
    }
}
// TODO: Possible problem with swinging when bending grass backward because growth vector changes
//  alternative motion capture to make sure vectors belong to each motion separatly
// 1. Put static vectors into properties
// 2. Put current vectors in each motion structure
// 3. Make structure Orientation with growth, normal, side vecs
// 4. CalcSimVectors now take Orentation structure instead of props
// 5. Dont recalculate growth etc vectors, rotate them every frame together with everything else

// Swinging motion is modeled by rotating groth vector about ground normal in dinw force
GrassPhysics::Rotation GrassPhysics::SwingMotion(tp::Real dt, const tp::Vec3 &wind_vec) {
    // In this motion growth vector is being replaced by growth projection vector on the ground plane
    //  because we want our rotation to be always about ground normal
    tp::Vec3 growth_proj_vec = util::math::GetXZProj(props_.growth_vec);
    tp::Vec3 growth_proj_vec_static = util::math::GetXZProj(props_static_.growth_vec);
    // Wind force
    // W = S * omega * (wind[dot]side) * side
    tp::Vec3 wind_force = props_.surf_area * props_.drag_coeff * glm::dot(wind_vec, props_.side_vec) * props_.side_vec;
    //util::Log::Write("M_grass: swing wind force ", glm::to_string(wind_force));
    // Restoration force
    // Use projections to calculate restoration force direction because other motions can offset current vector up or down
    //  but we don't care about that, we want to restore it back by rotating about ground normal
    // This section rotates static growth vector behind the current vector in order to avoid problems when
    //  rotation happens for more than 180 degrees. In that case rotation starts in oposite direction, which we dont want.
    tp::Vec3 restore_to_vec = growth_proj_vec_static;
    tp::Real adj_interval = M_PI_2;;
    if (swing_.displacement_ang > adj_interval) {
        tp::Real adjustemnt_ang = swing_.displacement_ang - adj_interval;
        restore_to_vec = glm::rotate(growth_proj_vec_static, adjustemnt_ang, util::math::kGroundNormal);
    } else 
    if (swing_.displacement_ang < -adj_interval) {
        tp::Real adjustemnt_ang = swing_.displacement_ang + adj_interval;
        restore_to_vec = glm::rotate(growth_proj_vec_static, adjustemnt_ang, util::math::kGroundNormal);
    }
    // R = K_tip * ang_displacement * normalize(growth_static - growth_current)
    tp::Vec3 restore_force = edges_.back().stiffness * abs(swing_.displacement_ang) * util::math::NormalizeSafe(restore_to_vec - growth_proj_vec);
    //util::Log::Write("M_grass: swing restore force ",glm::to_string(  restore_force ) );
    // Full force acting on a grass entity
    tp::Vec3 full_force = wind_force + restore_force;
    //util::Log::Write("M_grass: full force ",glm::to_string(  full_force ) );
    // To damp down motion when wind stops
    // FIXME: angular velocity speed approaches zero, but never becomes 0
    // TODO: put normal wind.stops functions
    if(!glm::length(wind_vec)) {
        tp::Real damping_coeff = -edges_.back().stiffness;
        swing_.angular_velocity += damping_coeff * swing_.angular_velocity * dt;
    }
    // Inertica
    // I = m * r^2;
    // Use real growth vec instead of projection because projection can be 0 if growth is straight up
    tp::Real root_tip_length = glm::length(props_.growth_vec);
    tp::Real inertia = props_.mass * root_tip_length * root_tip_length;
    assert(inertia > 0.0);
    // Torque
    // N = growth x full_force
    tp::Vec3 torque = glm::cross(growth_proj_vec, full_force);
    //util::Log::Write("M_grass: torque ",glm::to_string(  torque ) );
    // Angular acceleration
    tp::Vec3 angular_accel = torque / inertia;
    // Explicit Euler time integration
    swing_.angular_velocity += angular_accel * dt;
    // Rotation in this momentum by length of delta_theta vector about it
    tp::Vec3 delta_theta = swing_.angular_velocity * dt;
    // delta_theta changes it's rotation axis sign depending in which direction angle offset should rotate
    swing_.displacement_ang += glm::length(delta_theta) * (delta_theta.y >= 0.0 ? 1.0 : -1.0);
    //util::Log::Write("M_grass:ang acc ", glm::to_string(angular_accel), " ang vel ", glm::to_string(swing_.angular_velocity), " ang dis ", swing_.displacement_ang);
    Rotation rotation;
    // To return in convinient way we always rotate about ground normal but change angle sign depending on direction
    // If rotation is not fully about y axis, no need to continue
    assert(delta_theta.x == 0 && delta_theta.z == 0); 
    rotation.axis = util::math::kGroundNormal;
    rotation.angle = swing_.displacement_ang;
    return rotation;
}

// Bending motion is simulationg by rotating growth vector about its side vector
GrassPhysics::Rotation GrassPhysics::BendMotion(tp::Real dt, const tp::Vec3 &wind_vec) {
    // Wind force
    // W = S * omega * (wind[dot]normal) * normal
    tp::Vec3 wind_force = props_.surf_area * props_.drag_coeff * glm::dot(wind_vec, props_.normal_vec) * props_.normal_vec;
    //util::Log::Write("M_grass: bend wind force ", glm::to_string(wind_force));
    // Restore force
    // R = K_tip * ang_displacement * normalize(growth_static - growth_current)
    tp::Vec3 restore_to_vec = glm::rotate(props_static_.growth_vec, swing_.displacement_ang, util::math::kGroundNormal);
    tp::Vec3 restore_force = edges_.back().stiffness * abs(bend_.displacement_ang) * util::math::NormalizeSafe(restore_to_vec - props_.growth_vec);
    //util::Log::Write("M_grass: swing restore force ",glm::to_string(restore_force ));
    // Full force acting on a grass entity
    tp::Vec3 full_force = wind_force + restore_force;
    // To damp down motion when wind stops
    // FIXME: angular velocity speed approaches zero, but never becomes 0
    // TODO: put normal wind.stops functions
    if(!glm::length(wind_vec)) {
        tp::Real damping_coeff = -edges_.back().stiffness;
        bend_.angular_velocity += damping_coeff * bend_.angular_velocity * dt;
    }
    // Inertica
    // I = m * r^2;https://www.google.com/
    tp::Real root_tip_length = glm::length(props_.growth_vec);
    tp::Real inertia = props_.mass * root_tip_length * root_tip_length;
    assert(inertia > 0.0);
    // Torque
    // N = growth x full_force
    tp::Vec3 torque = glm::cross(props_.growth_vec, full_force);
    //util::Log::Write("M_grass: torque ",glm::to_string(  torque ) );
    // Angular acceleration
    tp::Vec3 angular_accel = torque / inertia;
    // Explicit Euler time integration
    bend_.angular_velocity += angular_accel * dt;
    // Rotation in this momentum by length of delta_theta vector about it
    tp::Vec3 delta_theta = bend_.angular_velocity * dt;
    // delta_theta changes it's rotation axis sign depending in which direction angle offset should rotate
    // FIXME: think how to determine sign when growth vector rotates
    bend_.displacement_ang += glm::length(delta_theta) * (delta_theta.x >= 0.0 ? 1.0 : -1.0);
    //util::Log::Write("M_grass:ang acc ", glm::to_string(angular_accel), " ang vel ", glm::to_string(swing_.angular_velocity), " ang dis ", swing_.displacement_ang);
    Rotation rotation;
    // To return in convinient way we always rotate about positive side axis but change angle sign depending on direction
    rotation.axis = util::math::NormalizeSafe(glm::abs(delta_theta));
    rotation.angle = bend_.displacement_ang;
    return rotation;
}

// Calculate set of fixed properties from edge data
void GrassPhysics::CalcFixedProperties(Properties &props, const std::vector<EdgeProperties> &edges) {
    // Mass
    props.mass = 0.0;
    for (const auto &edge : edges) {
        props.mass += edge.mass;
    }
    // Drag (experiment to get right)
    props.drag_coeff = 1.0;
    // Surface area
    props.surf_area = 0.0;
    for (auto edge_it = std::next(edges.cbegin()); edge_it != edges.cend(); ++edge_it) {
        auto prev_edge_it = std::prev(edge_it);
        // How long is section from this edge to previous one
        tp::Real section_length = glm::length(edge_it->position - prev_edge_it->position);
        tp::Real section_avarage_width = edge_it->width;
        props.surf_area += section_avarage_width * section_length;
        // util::Log::Write("M_grass: surface area ", props.surf_area);
    }
}

// Calculate growth normal and side vectors from given edges
void GrassPhysics::CalcSimVectors(Properties &props, const std::vector<EdgeProperties> &edges) {
    props.growth_vec = edges.back().position - edges.front().position;
    props.side_vec = util::math::NormalizeSafe(glm::cross(props.growth_vec, util::math::kGroundNormal));
    props.normal_vec = util::math::NormalizeSafe(glm::cross(props.side_vec, props.growth_vec));
}
}; // grass