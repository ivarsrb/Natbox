#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <engine/core/types.h>
#include <engine/renderer/types.h>

namespace app::study::grass {

class PlantNode {
public:
    PlantNode(const glm::tvec3<engine::tp::Real> &edge, const glm::tvec2<engine::tp::Real> &angle, const engine::tp::Real length);
    glm::tvec3<engine::tp::Real> origin;
private:
    glm::tvec2<engine::tp::Real> angle_;
    glm::tvec2<engine::tp::Real> static_angle_;
    engine::tp::Real length_;
};


class Plant {
public:
    Plant(const std::vector<glm::tvec3<engine::tp::Real>> &edges);
    void FillVertices(std::vector<engine::renderer::types::FPos> &vertices, uint32_t start_index);
    void Update(engine::tp::Real dt);
private:
    std::vector<PlantNode> nodes_;
};

};