#pragma once
#include <glad/glad.h>
#include <iostream>

namespace engine::renderer::gl {
class Shader {
public:
    // TODO - move this struture to render device
    enum class ShaderType : GLenum {
        kVertexShader = GL_VERTEX_SHADER,
        kTessControlShader = GL_TESS_CONTROL_SHADER,
        kTessEvaluationShader = GL_TESS_EVALUATION_SHADER,
        kGeometryShader = GL_GEOMETRY_SHADER,
        kFragmentShader = GL_FRAGMENT_SHADER,
        kComputeShader = GL_COMPUTE_SHADER
    };
    explicit Shader(ShaderType type);
    ~Shader();
    // move constructor
    Shader(Shader&& c) noexcept;
    // move assignment
    Shader& operator=(Shader&& c) noexcept;
    // Remove possibility to copy object
    Shader(Shader const&) = delete;
    Shader operator=(Shader const&) = delete;
    // Return Shader object name (handle)
    GLuint GetName() const;
    // Relaods shader source from file into shader object
    void LoadSource(const std::string shader_file);
    void Compile();
private:
    // Hide default constructor
    Shader();
    void Swap(Shader& a, Shader& b) noexcept;
    // Check and return status of shader compilation
    GLint CompilationStatus();
    // Return contents of the given file
    std::string ReadFile(const std::string file_path);
    bool own_ = false;
    // shader object handle
    GLuint name_ = 0;
};
}; // namespace gl

