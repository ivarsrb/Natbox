#pragma once
#include <glm/glm.hpp>

// Types concerning core components of the engine
// NOTE: renderer types are in renderer module
// Easier namespace for ease of use
namespace engine::tp {
//namespace engine::core::types {
// Type for floating point cariables where precision could be changed for testing or performance
using Real = double;
// Data that is common to all entities of thw world
struct WorldData {
    // How many meters one space unit represents
    // All objects are constructed in meters and later scaled by this scale to get how much space it really occupies
    //Real unit_scale;
};

}; // renderer