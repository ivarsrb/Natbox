#include "shader.h"
#include <vector>
#include <fstream>
#include <string>
#include <engine/util/log.h>

namespace engine::renderer::gl {
Shader::Shader(ShaderType type) {
    // could look into glCreateShaderProgram 
    name_ = glCreateShader(static_cast<GLenum>(type));
    own_ = true;
    util::Log::Write("Shader " , name_ , " of type " , static_cast<GLenum>(type) , " created");
}

Shader::~Shader() {
    if (own_) {
        util::Log::Write("Shader " , name_ , " destroyed (or marked for deletion if it's still attached to programm)");
        glDeleteShader(name_);
    }
}
  
// Move constructor
Shader::Shader(Shader &&c) noexcept {
    Swap(*this, c);
    util::Log::Write("Shader move constructed from ", name_, " to ", c.name_);
}

// Move assignment
Shader& Shader::operator=(Shader &&c) noexcept {
    Swap(*this, c);
    util::Log::Write("Shader move assigned from " , name_ , " to " , c.name_);
    return *this;
}

GLuint Shader::GetName() const {
    return name_;
}

void Shader::Swap(Shader &a, Shader &b) noexcept {
    // handle
    std::swap(a.name_, b.name_);
    // ownership
    std::swap(a.own_, b.own_);
}

// Loads or Relaods shader source from file into shader object
void Shader::LoadSource(const std::string shader_file) {
    std::string shader_string = ReadFile(shader_file);
    GLchar const *shader_source = shader_string.c_str();
    GLint const shader_length = (GLint)shader_string.size();
    glShaderSource(name_, 1, &shader_source, &shader_length);
    util::Log::Write("Shader " , name_ , " loaded source from file " , shader_file.c_str());
}

void Shader::Compile() {
    glCompileShader(name_);
    CompilationStatus();
}

// Check and return status of shader compilation
// Should be called after Compile() method
GLint Shader::CompilationStatus() {
    GLint status = 0;
    glGetShaderiv(name_, GL_COMPILE_STATUS, &status);
    util::Log::Write("Shader " , name_ , " compile status " , status);
    if(!status) {
        GLint max_length = 0;
        glGetShaderiv(name_, GL_INFO_LOG_LENGTH, &max_length);
        // The max_length includes the NULL character
        std::vector<GLchar> error_log(max_length);
        glGetShaderInfoLog(name_, max_length, &max_length, &error_log[0]);
        util::Log::Write2("Shader " , name_ , " compilation failed: " , &error_log[0]);
    }
    return status;
}

// To load shader source
std::string Shader::ReadFile(const std::string file_path) {
    std::string content;
    std::ifstream file_stream(file_path.c_str());
    if (file_stream.is_open()) {
        std::string line = "";
        for (std::string line; std::getline(file_stream, line);) {
            content.append(line + "\n");
        }
        file_stream.close();
    }
    else {
        throw std::runtime_error("(Shader) could not read file " + file_path);
        content = "";
    }
    return content;
}
}; // gl
