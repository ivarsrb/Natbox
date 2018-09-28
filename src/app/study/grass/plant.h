#pragma once
#include <vector>
#include "types.h"
#include <engine/core/types.h>
//#include <engine/util/math.h>
#include "plant_node.h"

namespace app::study::grass {

// Plant object holds plant nodes from which it is constructed
// Each plant node is animated independant and lives inside its own space with origin at (0,0,0)
// It is plant's responsibility to return correct plant edges given current animated plant nodes
class Plant {
public:
    Plant(const std::vector<engine::tp::Vec3> &edges);
    // Set all edges and nodes to it's original shape
    void Reset();
    void CreateNodesFromEdges(const std::vector<engine::tp::Vec3> &edges);
    void FillVertices(std::vector<VertexType> &vertices, uint32_t start_index) const;
    void Update(engine::tp::Real dt, const engine::tp::Vec3 &wind);
private:
    // Collection of plant nodes
    std::vector<PlantNode> nodes_;
    // Holds current edges of plant shape calculated from plant nodes current orientations
    std::vector<engine::tp::Vec3> edges_;
    // Edges that were given as original model
    std::vector<engine::tp::Vec3> static_edges_;
};

};