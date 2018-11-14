#pragma once
#include <vector>
#include <engine/core/types.h>

namespace app::study::grass {
// Simulation of animation of grass edges in dynamic wind force
// Bsed on paper "A simulation on grass swaying with dynamic wind force" (2016)
// TODO: 
// * add twisting motion
// * In bending motion need to make bended grass straighten out in strong wind (now it bends further)
//    to do that divide edges of grass in two groups depending on angle, then propagate 
//    motion accordingly
class Physics {
public:
    struct Edge {
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
    Physics(const std::vector<Edge> &edges);
    void Reset();
    // Return currently modified edges
    const std::vector<Edge>& GetTransformedEdges() const;
    void Update(engine::tp::Real dt, const engine::tp::Vec3 &wind_vec);
private:
    // Orientation of grass blade
    struct Orientation {
        // Vector from root to tip edge
        engine::tp::Vec3 growth_vec;
        // normal vector to growth vector
        engine::tp::Vec3 normal_vec;
        // Perpendicular to growth and normal
        engine::tp::Vec3 side_vec;
    };
    // Properties of a grass entity itself
    struct Properties {
        // Static vectors of a blade
        Orientation orientation;
        // Total mass of a entity (kg)
        engine::tp::Real mass;
        // Surface area (m)
        engine::tp::Real surf_area;
        // Drag coeficient of a surface (find experimentally)
        engine::tp::Real drag_coeff;
    };
    // Data about given motion of a grass in wind force
    struct Motion {
        // Dynamic vectors of a blade in given motion
        Orientation orientation;
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
    // Calculate and return set of fixed properties from edge data
    void GetPropertiesFromEdges(Properties &props, const std::vector<Edge> &edges);
    // Calculate and return growth normal and side vectors from given edges
    void GetOrientationFromEdges(Orientation &orient, const std::vector<Edge> &edges);
    // Calculatate independent motions
    Rotation Swing(engine::tp::Real dt, const engine::tp::Vec3 &wind_vec);
    Rotation Bend(engine::tp::Real dt, const engine::tp::Vec3 &wind_vec);
    // Static variables are initial data of a grass entity and does not change
    // Edges
    std::vector<Edge> edges_;
    std::vector<Edge> edges_static_;
    // Grass entity itself
    Properties props_;
    // Motions
    Motion swing_;
    Motion bend_;
};
}; // grass