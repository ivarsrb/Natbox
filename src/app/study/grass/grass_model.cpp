#include "grass_model.h"
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <nlohmann/json.hpp>
#include <engine/util/math.h>
#include "config.h"

namespace app::study::grass {
GrassModel::GrassModel(const std::string &model_file) {
    LoadFromFile(model_file);
    Reset();
}
// Set model in it's initial state
void GrassModel::Reset() {
    edges_ = edges_static_;
}

void GrassModel::UpdateOrientationVectors() {
    ComputeOrientationVectors(edges_);
}

// Load model data from json file
void GrassModel::LoadFromFile(const std::string &file_name) {
    std::ifstream model_file(std::string(MODELS_PATH) + file_name);
    if (!model_file.fail()) {
        nlohmann::json model_data = nlohmann::json::parse(model_file);
        // Load edge data
        for (const auto &model_edge : model_data["edges"]) {
            Edge edge;
            model_edge["position"][0].get_to(edge.position.x);
            model_edge["position"][1].get_to(edge.position.y);
            model_edge["position"][2].get_to(edge.position.z);
            // Infinity is represented by null in file
            if (model_edge["stiffness"].is_null()) {
                edge.stiffness = std::numeric_limits<float>::infinity();
            }
            else {
                model_edge["stiffness"].get_to(edge.stiffness);
            }
            edges_static_.push_back(edge);
        }
        // Load whole data
        model_data["mass"].get_to(mass_);
        model_data["width"].get_to(width_);
        ComputeOrientationVectors(edges_static_);
        //std::cout << model_data.dump(4) << std::endl;
    } else {
        throw std::runtime_error("(Grass model) could not open file " + file_name);
    }
}

// Calculate and set orientation vectors for each edge given edge positions
void GrassModel::ComputeOrientationVectors(std::vector<Edge> &edges) {
    for (auto edge_it = edges.begin(); edge_it != edges.end(); ++edge_it) {
        if (std::next(edge_it) != edges.end()) {
            auto next_edge_it = std::next(edge_it);
            edge_it->edge_vec = next_edge_it->position - edge_it->position;
            // This side vectors is used to determine normal of an edge
            // we are assuming hat initially no edge can be twisted
            const auto refrence_side_vec = engine::tp::Vec3(1.0, 0.0, 0.0); 
            edge_it->normal_vec = engine::util::math::NormalizeSafe(glm::cross(edge_it->edge_vec, refrence_side_vec));
            edge_it->side_vec = engine::util::math::NormalizeSafe(glm::cross(edge_it->edge_vec, edge_it->normal_vec));
        } else {
            // For the last one
            auto prev_edge_it = std::prev(edge_it);
            edge_it->edge_vec = engine::tp::Vec3(0.0);
            edge_it->normal_vec = prev_edge_it->normal_vec;
            edge_it->side_vec = prev_edge_it->side_vec;;
        }   
        //std::cout << "edge  " << glm::to_string(edge_it->edge_vec) << "\n";
        //std::cout << "normal  " << glm::to_string(edge_it->normal_vec) << "\n";
        //std::cout << "side  " << glm::to_string(edge_it->side_vec) << "\n";
    }
}
}; // grass