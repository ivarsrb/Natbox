#pragma once
#include <engine/core/types.h>
#include "grass_model.h"

namespace app::study::grass {
class GrassPhysics {
public:
    GrassPhysics(const std::vector<GrassModel::Edge> &edges, const engine::tp::Real mass, const engine::tp::Real width);
    void Update(engine::tp::Real dt, std::vector<GrassModel::Edge> &edges, const std::vector<GrassModel::Edge> &static_edges, const engine::tp::Vec3 &wind_vec);
    void Reset();
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
    void GetPropertiesFromEdges(Properties &props, const std::vector<GrassModel::Edge> &edges, const engine::tp::Real mass, const engine::tp::Real width);
    // Calculate and return growth normal and side vectors from given edges
    void GetOrientationFromEdges(Orientation &orient, const std::vector<GrassModel::Edge> &edges);
    // Calculatate independent motions
    Rotation Swing(engine::tp::Real dt, const engine::tp::Vec3 &wind_vec, const std::vector<GrassModel::Edge> &edges);
    Rotation Bend(engine::tp::Real dt, const engine::tp::Vec3 &wind_vec, const std::vector<GrassModel::Edge> &edges);
    // Properties of whole grass model needed for simulation
    Properties props_;
    // Motions
    Motion swing_;
    Motion bend_;
};
}; // grass