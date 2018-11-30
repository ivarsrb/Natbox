#pragma once
#include <vector>
#include <string>
#include <engine/core/types.h>

namespace app::study::grass {
// Stores grass model data
class GrassModel {
public:
    // Each edge is a vertex of a grass model
    // It's vectors re determined by next edges positions
    struct Edge {
        // local position
        engine::tp::Vec3 position;
        engine::tp::Real stiffness;
        // From this to next edge
        // does not exist for last edge
        engine::tp::Vec3 edge_vec;
        // Normal vector of section from this to next edge 
        // for the last edge keep the one from previous edge
        engine::tp::Vec3 normal_vec;
        // Perpendicular to normal and edge vector
        // for the last edge keep the one from previous edge
        engine::tp::Vec3 side_vec;
    };
    GrassModel(const std::string &model_file);
    // Set model in it's initial state
    void Reset();
    void UpdateOrientationVectors();
    // Returns reference of current model edges
    std::vector<Edge>& GetEdges() {return edges_;};
    const std::vector<Edge>& GetEdges() const {return edges_;};
    const std::vector<Edge>& GetStaticEdges() const {return edges_static_;};
    const engine::tp::Real GetMass() const {return mass_;};
    const engine::tp::Real GetWidth() const {return width_;};
private:
    // Load model data from json file
    void LoadFromFile(const std::string &file_name);
    // Calculate and set orientation vectors for each edge given edge positions
    void ComputeOrientationVectors(std::vector<Edge> &edges);
    // Are initialized once and never change
    // representing static state of a model
    std::vector<Edge> edges_static_;
    // Edges that are modified by physical transformations and rendered
    std::vector<Edge> edges_;
    // Mass of the blade
    engine::tp::Real mass_;
    // Width of the blade along side vector
    engine::tp::Real width_;
};
}; // grass