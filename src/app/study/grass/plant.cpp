#include "plant.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

using namespace engine;
namespace app::study::grass {

PlantNode::PlantNode(const glm::tvec3<tp::Real> &edge, const glm::tvec2<tp::Real> &angle, const tp::Real length):
                    origin(edge), angle_(angle), length_(length) {
    static_angle_ = angle_;
}

Plant::Plant(const std::vector<glm::tvec3<tp::Real>> &edges) {
    for (auto edge_it = edges.cbegin(); edge_it != edges.cend(); ++edge_it) {
        auto next_edge_it = std::next(edge_it);
        tp::Real node_length = 0.0;
        glm::tvec2<tp::Real> rot_angle(0.0,0.0);
        if(next_edge_it != edges.end()) {
            node_length = glm::distance(*edge_it, *next_edge_it);
            rot_angle.x = glm::angle( glm::normalize(*next_edge_it - *edge_it), glm::tvec3<tp::Real>(1.0,0.0,0.0));
            rot_angle.y = glm::angle( glm::normalize(*next_edge_it - *edge_it), glm::tvec3<tp::Real>(0.0,0.0,1.0));
        }
        // Final is a dummy node that stores only origin but does not have length
        nodes_.emplace_back(*edge_it, rot_angle, node_length);
        //std::cout << "----------- " << glm::degrees(rot_angle.x) << " " << glm::degrees(rot_angle.y) << "\n";
    }
}

void Plant::FillVertices(std::vector<renderer::types::FPos> &vertices, uint32_t start_index) {
    for (const auto& node : nodes_) {
        vertices.at(start_index++).position = node.origin;
    }
}

void Plant::Update(tp::Real dt) {
    /*
    int a = 0;
    for (auto& node : nodes_) {
        if (a > 0)
            node.origin += glm::tvec3<tp::Real>(1.0, 0.0, 0.0)  * dt;
        a++;
    }
    */
}

};