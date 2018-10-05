#pragma once
#include <vector>
#include "types.h"
#include <engine/core/types.h>
//#include "plant_node.h"

namespace app::study::grass {

class Plant {
public:
    Plant(const std::vector<engine::tp::Vec3> &edges);
    // Set all edges and nodes to it's original shape
    void Reset();
    //void CreateNodesFromEdges(const std::vector<engine::tp::Vec3> &edges);
    void FillVertices(std::vector<VertexType> &vertices, uint32_t start_index) const;
    void Update(engine::tp::Real dt, const engine::tp::Vec3 &wind);
private:
    // Get normal and normalized side vectors given growth vector
    void GetNormalSideVectors(const engine::tp::Vec3 &growth, engine::tp::Vec3 &normal, engine::tp::Vec3 &side) const;

    // Collection of plant nodes
    //std::vector<PlantNode> nodes_;
    // Holds current edges of plant shape calculated from plant nodes current orientations
    std::vector<engine::tp::Vec3> edges_;
    // Edges that were given as original model
    std::vector<engine::tp::Vec3> static_edges_;
    // Vector from origin to final edge
    engine::tp::Vec3 growth_vec_;
    engine::tp::Vec3 growth_static_vec_;
    // Should be normalized
    engine::tp::Vec3 normal_vec_;
    engine::tp::Vec3 normal_static_vec_;
    // Should be normalized
    engine::tp::Vec3 side_vec_;
    engine::tp::Vec3 side_static_vec_;

    engine::tp::Vec3 angular_velocity_;
    //engine::tp::Real angular_displacement_;
};

};