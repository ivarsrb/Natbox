#include "grass_physics.h"
#include <glm/glm.hpp>
#include <engine/util/math.h>

namespace app::study::grass {
GrassPhysics::GrassPhysics(const std::vector<GrassModel::Edge> &edges, const engine::tp::Real mass, const engine::tp::Real width) {
    // By definition edge count is always 4
    constexpr uint32_t kNumEdges = 4;
    assert(edges.size() == kNumEdges);
    // Models should start at origin
    assert(!glm::length(edges.front().position));
    // Accept only models that face positive z dirrection
    assert(edges.front().position.x == edges.back().position.x);
    assert(edges.back().position.z > 0);

    GetPropertiesFromEdges(props_, edges, mass, width);
    Reset();
}

// Reset simulation to it's initial state
void GrassPhysics::Reset() {
    // Motions
    swing_.orientation = props_.orientation;
    swing_.angular_velocity = engine::tp::Vec3(0.0);
    swing_.displacement_ang = 0;
    bend_.orientation = props_.orientation;
    bend_.angular_velocity = engine::tp::Vec3(0.0);
    bend_.displacement_ang = 0;
}

// Logic: grass blade consists of 4 edges. Motion is calculated only for top edge, all other
//        edges are calculated as propogation of motion toward grass root (1st edge)
// Several motions are combined together to form final motion
// Final motions consists of independant motions added together by edge rotation
void GrassPhysics::Update(engine::tp::Real dt, std::vector<GrassModel::Edge> &edges, const std::vector<GrassModel::Edge> &static_edges, const engine::tp::Vec3 &wind_vec) {
    // TODO: maybe make motions concatination beautifull
    // Bend
    engine::tp::Vec3 adjusted_wind_vec = engine::util::math::RotateSafe(wind_vec, -swing_.displacement_ang, engine::util::math::kGroundNormal);
    Rotation rotation_b = Bend(dt, adjusted_wind_vec, edges);
    for (uint32_t i = 1; i < edges.size(); ++i) {
        // Rotate each sections as a fraction of total rotation
        //current_angle = rotation_b.angle
        engine::tp::Real current_angle = (edges.back().stiffness / edges.at(i).stiffness) * rotation_b.angle;  
        edges.at(i).position = engine::util::math::RotateSafe(static_edges.at(i).position, current_angle, rotation_b.axis);
    }
    
    // Swing
    Rotation rotation_s = Swing(dt, wind_vec, edges);
    for (uint32_t i = 1; i < edges.size(); ++i) {  
        // Rotate each sections as a fraction of total rotation
        //current_angle = rotation_s.angle
        engine::tp::Real current_angle = (edges.back().stiffness / edges.at(i).stiffness) * rotation_s.angle;  
        edges.at(i).position = engine::util::math::RotateSafe(edges.at(i).position, current_angle, rotation_s.axis);
    }
}

// Rotate always about x axis, adjust wind according to bend effect
GrassPhysics::Rotation GrassPhysics::Bend(engine::tp::Real dt, const engine::tp::Vec3 &wind_vec, const std::vector<GrassModel::Edge> &edges) {
    // We are not rotating vectors, we adjust wind acoording to current swing
    //tp::Vec3 wind_adjusted = glm::rotate(wind_vec, -swing_.displacement_ang, util::math::kGroundNormal);
    // Wind force
    // W = S * omega * (wind[dot]normal) * normal
    engine::tp::Vec3 wind_force = props_.surf_area * props_.drag_coeff * glm::dot(wind_vec, bend_.orientation.normal_vec) * bend_.orientation.normal_vec;
    //util::Log::Write("M_grass: bend wind force ", glm::to_string(wind_force));
    // Restore force
    // R = K_tip * ang_displacement * normalize(growth_static - growth_current)
    // In case this breaks often, put either restrictions or adjust restore_to vector like in swing motion
    assert(abs(bend_.displacement_ang) < M_PI);
    engine::tp::Vec3 restore_force = edges.back().stiffness * abs(bend_.displacement_ang) * engine::util::math::NormalizeSafe(props_.orientation.growth_vec - bend_.orientation.growth_vec);
    //util::Log::Write("M_grass: bend restore force ",glm::to_string(restore_force ));
    //util::Log::Write("M_grass: Normalize ", glm::to_string(glm::normalize(props_.orientation.growth_vec - bend_.orientation.growth_vec)));
    //util::Log::Write("M_grass: stat growth ", glm::to_string(props_.orientation.growth_vec - bend_.orientation.growth_vec));
    //util::Log::Write("M_grass: curr growth ", glm::to_string(bend_.orientation.growth_vec));
    // Full force acting on a grass entity
    engine::tp::Vec3 full_force = wind_force + restore_force;
    //tp::Vec3 full_force = tp::Vec3(0.0);
    //util::Log::Write("M_grass: full force ",glm::to_string(full_force));
    // To damp down motion when wind stops
    // FIXME: angular velocity speed approaches zero, but never becomes 0
    // TODO: put normal wind.stops functions
    if (!glm::length(wind_vec)) {
        engine::tp::Real damping_coeff = -edges.back().stiffness;
        bend_.angular_velocity += damping_coeff * bend_.angular_velocity * dt;
    }
    // Inertica
    // I = m * r^2
    engine::tp::Real root_tip_length = glm::length(props_.orientation.growth_vec);
    engine::tp::Real inertia = props_.mass * root_tip_length * root_tip_length;
    assert(inertia > 0.0);
    // Torque
    // N = growth x full_force
    engine::tp::Vec3 torque = glm::cross(bend_.orientation.growth_vec, full_force);
    //util::Log::Write("M_grass: torque ",glm::to_string(  torque ) );
    // Angular acceleration
    engine::tp::Vec3 angular_accel = torque / inertia;
    // Explicit Euler time integration
    bend_.angular_velocity += angular_accel * dt;
    //util::Log::Write("M_grass: ang vel ",glm::to_string(  bend_.angular_velocity ) );
    // Rotation in this momentum by length of delta_theta vector about it
    engine::tp::Vec3 delta_theta = bend_.angular_velocity * dt;
    // delta_theta changes it's rotation axis sign depending in which direction angle offset should rotate
    bend_.displacement_ang += glm::length(delta_theta) * (delta_theta.x >= 0.0 ? 1.0 : -1.0);
    //util::Log::Write("M_grass:ang acc ", glm::to_string(angular_accel), " ang vel ", glm::to_string(bend_.angular_velocity), " ang dis ", bend_.displacement_ang);
    Rotation rotation;
    // To return in convinient way we always rotate about positive side axis but change angle sign depending on direction
    // Bend rotates as if model faces Z direction
    rotation.axis = engine::tp::Vec3(1.0,0.0,0.0); //util::math::NormalizeSafe(glm::abs(delta_theta));
    rotation.angle = bend_.displacement_ang;
    //util::Log::Write("M_grass: length ",glm::length(rotation.axis ));
    //util::Log::Write("M_grass: rot axis ", glm::to_string(rotation.axis), " angle ", rotation.angle);
    // Update orientation vectors
    bend_.orientation.growth_vec = engine::util::math::RotateSafe(props_.orientation.growth_vec, rotation.angle, rotation.axis);
    bend_.orientation.normal_vec = engine::util::math::RotateSafe(props_.orientation.normal_vec, rotation.angle, rotation.axis);
    //util::Log::Write("M_grass: growth ", glm::to_string(bend_.orientation.growth_vec ));

    return rotation;
}

// In this motion growth vector is being replaced by growth projection vector on the ground plane
GrassPhysics::Rotation GrassPhysics::Swing(engine::tp::Real dt, const engine::tp::Vec3 &wind_vec, const std::vector<GrassModel::Edge> &edges) {
    //  because we want our rotation to be always about ground normal
    engine::tp::Vec3 growth_proj_vec = engine::util::math::GetXZProj(swing_.orientation.growth_vec);
    engine::tp::Vec3 growth_proj_vec_static = engine::util::math::GetXZProj(props_.orientation.growth_vec);
    // Wind force
    // W = S * omega * (wind[dot]side) * side
    engine::tp::Vec3 wind_force = props_.surf_area * props_.drag_coeff * glm::dot(wind_vec, swing_.orientation.side_vec) * swing_.orientation.side_vec;
    // Restoration force
    // Use projections to calculate restoration force direction because other motions can offset current vector up or down
    //  but we don't care about that, we want to restore it back by rotating about ground normal
    // This section rotates static growth vector behind the current vector in order to avoid problems when
    //  rotation happens for more than 180 degrees. In that case rotation starts in oposite direction, which we dont want.
    engine::tp::Vec3 restore_to_vec = growth_proj_vec_static;
    engine::tp::Real adj_interval = M_PI_2;;
    if (swing_.displacement_ang > adj_interval) {
        engine::tp::Real adjustemnt_ang = swing_.displacement_ang - adj_interval;
        restore_to_vec = engine::util::math::RotateSafe(growth_proj_vec_static, adjustemnt_ang, engine::util::math::kGroundNormal);
    } else 
    if (swing_.displacement_ang < -adj_interval) {
        engine::tp::Real adjustemnt_ang = swing_.displacement_ang + adj_interval;
        restore_to_vec = engine::util::math::RotateSafe(growth_proj_vec_static, adjustemnt_ang, engine::util::math::kGroundNormal);
    }
    // R = K_tip * ang_displacement * normalize(growth_static - growth_current)
    engine::tp::Vec3 restore_force = edges.back().stiffness * abs(swing_.displacement_ang) * engine::util::math::NormalizeSafe(restore_to_vec - growth_proj_vec);
    // Full force acting on a grass entity
    engine::tp::Vec3 full_force = wind_force + restore_force;
    // To damp down motion when wind stops
    // FIXME: angular velocity speed approaches zero, but never becomes 0
    // TODO: put normal wind.stops functions (if needed)
    if (!glm::length(wind_vec)) {
        engine::tp::Real damping_coeff = -edges.back().stiffness;
        swing_.angular_velocity += damping_coeff * swing_.angular_velocity * dt;
    }
    // Inertica
    // I = m * r^2
    // Use real growth vec instead of projection because projection can be 0 if growth is straight up
    engine::tp::Real root_tip_length = glm::length(props_.orientation.growth_vec);
    engine::tp::Real inertia = props_.mass * root_tip_length * root_tip_length;
    assert(inertia > 0.0);
    // Torque
    // N = growth x full_force
    engine::tp::Vec3 torque = glm::cross(growth_proj_vec, full_force);
    //util::Log::Write("M_grass: torque ",glm::to_string(  torque ) );
    // Angular acceleration
    engine::tp::Vec3 angular_accel = torque / inertia;
    // Explicit Euler time integration
    swing_.angular_velocity += angular_accel * dt;
    // Rotation in this momentum by length of delta_theta vector about it
    engine::tp::Vec3 delta_theta = swing_.angular_velocity * dt;
    // delta_theta changes it's rotation axis sign depending in which direction angle offset should rotate
    swing_.displacement_ang += glm::length(delta_theta) * (delta_theta.y >= 0.0 ? 1.0 : -1.0);
    //util::Log::Write("M_grass:ang acc ", glm::to_string(angular_accel), " ang vel ", glm::to_string(swing_.angular_velocity), " ang dis ", swing_.displacement_ang);
    Rotation rotation;
    // To return in convinient way we always rotate about ground normal but change angle sign depending on direction
    // If rotation is not fully about y axis, no need to continue
    assert(delta_theta.x == 0 && delta_theta.z == 0); 
    rotation.axis = engine::util::math::kGroundNormal;
    rotation.angle = swing_.displacement_ang;
    // Update orientation vectors
    swing_.orientation.growth_vec = engine::util::math::RotateSafe(props_.orientation.growth_vec, rotation.angle, rotation.axis);
    swing_.orientation.side_vec = engine::util::math::RotateSafe(props_.orientation.side_vec, rotation.angle, rotation.axis);

    return rotation;
}

// Calculate and return set of fixed properties from edge data
void GrassPhysics::GetPropertiesFromEdges(Properties &props, const std::vector<GrassModel::Edge> &edges, const engine::tp::Real mass, const engine::tp::Real width) {
    // Mass
    props.mass = mass;

    // Drag (experiment to get right)
    props.drag_coeff = 1.0;
    // Surface area
    props.surf_area = 0.0;
    for (auto edge_it = std::next(edges.cbegin()); edge_it != edges.cend(); ++edge_it) {
        auto prev_edge_it = std::prev(edge_it);
        // How long is section from this edge to previous one
        engine::tp::Real section_length = glm::length(edge_it->position - prev_edge_it->position);
        props.surf_area += width * section_length;
        // util::Log::Write("M_grass: surface area ", props.surf_area);
    }
    GetOrientationFromEdges(props.orientation, edges);
}

// Calculate and return growth normal and side vectors from given edges
// FIXME: when blade is straight up we can not determine its orientation (sode vec, normal vec), and we get error
// Probably need to get orentation straight from entity itself  or forbid to have fully straight-up grass model
// The same problem occurs when tip is slightly offset to left or right, orientation vector is not straight and bending motion screws up
void GrassPhysics::GetOrientationFromEdges(Orientation &orient, const std::vector<GrassModel::Edge> &edges) { 
    orient.growth_vec = edges.back().position - edges.front().position;
    orient.side_vec = engine::util::math::NormalizeSafe(glm::cross(orient.growth_vec, engine::util::math::kGroundNormal));
    orient.normal_vec = engine::util::math::NormalizeSafe(glm::cross(orient.side_vec, orient.growth_vec));
    //util::Log::Write("M_grass: growth vec ", glm::to_string(orient.growth_vec));
    //util::Log::Write("M_grass: side vec ", glm::to_string(orient.side_vec));
    //util::Log::Write("M_grass: normal vec ", glm::to_string(orient.normal_vec));
}
}; // grass