#pragma once
#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include "shader.h"
namespace engine::renderer::gl {
// Shader program object
class Program {
public:
    Program();
    ~Program();
    // move constructor
    Program(Program&& c) noexcept;
    // move assignment
    Program& operator=(Program&& c) noexcept;
    // Remove possibility to copy object
    Program(Program const&) = delete;
    Program operator=(Program const&) = delete;
    // Return program object name (handle)
    GLuint GetName() const;
    // Attach shader to program
    void AttachShader(const Shader &shader);
    // Detach shader from program
    void DetachShader(const Shader & shader);
    // Link attached shader objects into executable
    void Link();
    // Check and return status of program linking
    GLint LinkStatus();
    // Installs a program object as part of current rendering state
    void Use();
    // Returns the location of an attribute variable
    GLint GetAttribLocation(const std::string &attrib_name) const;
    // Returns the location of an uniform variable
    GLint GetUniformLocation(const std::string &uniform_name) const;
    // Setting uniform variables
    void SetBool(const std::string & name, bool value) const;
    void SetInt(const std::string & name, int value) const;
    void SetFloat(const std::string & name, float value) const;
    void SetVec3(const std::string & name, const glm::vec3 & value) const;
    void SetVec3(const std::string & name, float x, float y, float z) const;
    void SetMat4(const std::string & name, const glm::mat4 & mat) const;
private:
    void Swap(Program& a, Program& b) noexcept;
    bool own_ = false;
    // program object handle
    GLuint name_ = 0;
};
}; // gl
