#pragma once
#include <vector>
#include <engine/core/types.h>

namespace app::study::grass {
// Simulation of animation of grass edges in dynamic wind force
class GrassPhysics {
public:
    struct EdgeProperties {
        // Properties of each edge for simulation
        // Usually edge property means also property of a section from previous
        //  edge to this edge
        engine::tp::Vec3 position;
        // avarage width of a grass blade section from prev. edge to this edge
        engine::tp::Real width;
        // How stiff this adge will rotate about previous edge, 
        // NOT stiffness of rotation about this edge
        engine::tp::Real stiffness;
        // Mass of section up to this edge in kg
        engine::tp::Real mass;
    };
    GrassPhysics(const std::vector<EdgeProperties> &edges);
    void Reset();
    // Return currently modified edges
    const std::vector<EdgeProperties>& CurrentEdges() const;
    void Update(engine::tp::Real dt, const engine::tp::Vec3 &wind_vec);
private:
    // Properties of a grass entity itself
    struct Properties {
        // Vector from root to tip edge
        engine::tp::Vec3 growth_vec;
        // normal vector to growth vector
        engine::tp::Vec3 normal_vec;
        // Perpendicular to growth and normal
        engine::tp::Vec3 side_vec;
        // Total mass of a entity (kg)
        engine::tp::Real mass;
        // Surface area (m)
        engine::tp::Real surf_area;
        // Drag coeficient of a surface (find experimentally)
        engine::tp::Real drag_coeff;
    };
    // Data about given motion of a grass in wind force
    struct MotionData {
        // Velocity accumulates over simulation process
        engine::tp::Vec3 angular_velocity;
        // Angle by whitch current motion rotates grass entity from it's static position
        engine::tp::Real displacement_ang;
    };
    // Used to store and transfer rotation data
    struct Rotation {
        engine::tp::Vec3 axis;
        engine::tp::Real angle;
    };
    // Calculate set of fixed properties from edge data
    void CalcFixedProperties(Properties &props, const std::vector<EdgeProperties> &edges);
    // Calculate growth normal and side vectors from given edges
    void CalcSimVectors(Properties &props, const std::vector<EdgeProperties> &edges);
    Rotation SwingMotion(engine::tp::Real dt, const engine::tp::Vec3 &wind_vec);
    Rotation BendMotion(engine::tp::Real dt, const engine::tp::Vec3 &wind_vec);
    // Static variables are initial data of a grass entity and does not change
    // Edges
    std::vector<EdgeProperties> edges_;
    std::vector<EdgeProperties> edges_static_;
    // Grass entity itself
    Properties props_;
    Properties props_static_;
    // Motions
    MotionData swing_;
    MotionData bend_;
};
}; // grass