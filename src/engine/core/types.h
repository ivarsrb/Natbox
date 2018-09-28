#pragma once
#include <glm/glm.hpp>

// Types concerning core components of the engine or any frequently used types
// NOTE: renderer types are in renderer module
// Easier namespace for ease of use
namespace engine::tp {
// Type for floating point cariables where precision could be changed for testing or performance
using Real = double;
// Commonly used types
using Vec3 = glm::tvec3<engine::tp::Real>;
using Vec2 = glm::tvec2<engine::tp::Real>;

}; // renderer