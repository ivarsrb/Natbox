#pragma once
#include <glm/glm.hpp>

// App specific data types
// Data meant to be snet as shader uniforms
namespace app::study::terrain {
struct ShaderData {
    glm::mat4 view_from_world;
    glm::mat4 projection_from_view;
    glm::mat4 world_from_local;
};
};
