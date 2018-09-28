#include "plant.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL 
#include <glm/gtx/string_cast.hpp>

using namespace engine;

namespace app::study::grass {

// NOTE: First point of edges array should always be a root of a plant
Plant::Plant(const std::vector<tp::Vec3> &edges): static_edges_(edges) {
    Reset();
    /*
    for (auto edge_it = edges.cbegin(); edge_it != edges.cend(); ++edge_it) {
        auto next_edge_it = std::next(edge_it);
        util::Math::SphericCoords node_coord;
        if (next_edge_it != edges.end()) {
            tp::Pos3D node_vec = *next_edge_it - *edge_it;
            node_coord = util::Math::CartesianToSpheric(node_vec);
        }
        // Final is a dummy node that stores only origin but does not have length
        nodes_.emplace_back(*edge_it, node_coord);
        //std::cout << "node- r:" << node_coord.radius << " theta:" << glm::degrees(node_coord.theta) << " phi:" << glm::degrees(node_coord.phi) << "\n";
    }
    */
}

// Set all edges and nodes to it's original shape
void Plant::Reset() {
    edges_ = static_edges_;
    nodes_.clear();
    CreateNodesFromEdges(edges_);
}

// Calculate vector for each section and create node given this vector in cartesian coordinates
// NOTE: order of nodes is important, it should match order of edges
void Plant::CreateNodesFromEdges(const std::vector<tp::Vec3> &edges) {
    for (auto edge_it = edges.cbegin(); edge_it != std::prev(edges.cend()); ++edge_it) {
        auto next_edge_it = std::next(edge_it);
        tp::Vec3 node_vec = *next_edge_it - *edge_it;
        nodes_.emplace_back(node_vec);
    }
    /*
    for (const auto& node : nodes_) {
        node.Print();
    }
    */
}

void Plant::FillVertices(std::vector<VertexType> &vertices, uint32_t start_index) const {
    for (const auto& edge : edges_) {
        vertices.at(start_index++).position = edge;
    }
}

// Animate nodes and calculate new edge points of a plant one by one starting at first after the root
void Plant::Update(tp::Real dt, const tp::Vec3 &wind) {
    // Since first edge of a plant is root, it always remains the same and we dont update it
    uint32_t edge_num = 1;
    for (auto& node : nodes_) {
        node.Update(dt, wind);
        edges_.at(edge_num) = edges_.at(edge_num - 1) + node.Vector();
        edge_num++;
    }
}
};