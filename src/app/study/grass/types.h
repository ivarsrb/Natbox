#pragma once
#include <glm/glm.hpp>
#include <engine/renderer/types.h>

// App specific data types
// Data meant to be snet as shader uniforms
namespace app::study::grass {
struct ShaderData {
    glm::mat4 view_from_world;
    glm::mat4 projection_from_view;
    glm::mat4 world_from_local;
};
using VertexType = engine::renderer::types::FPos;
};