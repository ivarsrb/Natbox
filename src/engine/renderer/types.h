#pragma once
//#include <exception>
#include <stdexcept>
// For opengl types (could include opengl header)
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Types concerning renderer
namespace engine::renderer::types {
// Vertex composition Structures
// F - types of values are store in float
struct FPos {
    glm::vec3 position;
};
struct FPosColor {
    glm::vec3 position;
    glm::vec3 color;
};
struct FPosNormal {
    glm::vec3 position;
    glm::vec3 normal;
};
struct FPosTex {
    glm::vec3 position;
    glm::vec2 tex;
};
struct FPos2Tex {
    glm::vec2 position;
    glm::vec2 tex;
};
struct FPosNormalTex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex;
};

// OpenGL built-in type abstraction
// These type indexes should match ogl_types indexes
enum RenderDataTypes {
    kByte = 0,
    kShort,
    kInt,
    kUByte,
    kUShort,
    kUInt,
    kHalfFloat,
    kFloat,
    kDouble
};
// Converts renderer type constant to OpenGL constant
inline GLenum ToApiType(const RenderDataTypes type) {
    const GLenum ogl_types[] = { GL_BYTE, GL_SHORT, GL_INT, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT, GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE };
    if (type < 0 || type >= (sizeof(ogl_types) / sizeof(ogl_types[0]))) {
        throw std::out_of_range("(Types) API type out of range!");
    }
    return ogl_types[type];
}

// Structure to describe vertex attributes for VAO
struct VertexAttributeDescr {
    // Usually comes from layout(location = n) in vertex shader
    uint32_t attribute_index; 
    // The number of values per vertex attribute that are stored in the array
    int32_t size; // GLint
                  // The type of the data stored in the array
    types::RenderDataTypes type; // GLenum
                                 // The distance between attributes within the buffer.
    uint32_t offset; //GLuint
                     // The distance between vertex elements within the buffer (will be the same for all atributes because it's used at vbo binding)
                     // In previous OpenGL versions stride was part of vertex attribute so leave it here
    int32_t stride; //GLsizei
};
}; // renderer


