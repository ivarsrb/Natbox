#pragma once
#include <glm/glm.hpp>
#include <engine/renderer/types.h>

// App specific data types
// Data meant to be snet as shader uniforms
namespace app::study::grass {

// TODO: maybe this should be class with functions that set eachh member in buffer
struct UniformData {
    glm::mat4 view_from_world;
    glm::mat4 projection_from_view;
    glm::mat4 world_from_local;
};
using VertexType = engine::renderer::types::FPos;
};