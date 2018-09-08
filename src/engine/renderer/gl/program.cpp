#include "program.h"
#include <vector>
#include <engine/util/log.h>

namespace engine::renderer::gl {
Program::Program() {
    name_ = glCreateProgram();
    own_ = true;
    util::Log::Write("Program " , name_ , " created");
}

Program::~Program() {
    if (own_) {
        util::Log::Write("Program " , name_ , " destroyed");
        glDeleteProgram(name_);
    }
}

// Move constructor
Program::Program(Program &&c) noexcept {
    Swap(*this, c);
    util::Log::Write("Program move constructed from " , name_ , " to " , c.name_);
}

// Move assignment
Program& Program::operator=(Program &&c) noexcept {
    Swap(*this, c);
    util::Log::Write("Program move assigned from " , name_ , " to " , c.name_);
    return *this;
}

GLuint Program::GetName() const {
    return name_;
}

void Program::Swap(Program & a, Program & b) noexcept {
    // handle
    std::swap(a.name_, b.name_);
    // ownership
    std::swap(a.own_, b.own_);
}

// Attach shader to program
void Program::AttachShader(const Shader &shader) {
    glAttachShader(name_, shader.GetName());
    util::Log::Write("Program " , name_ , " attached shader " , shader.GetName());
}

// Detach shader from program
// Shader will not be deleted unless it is detached first, if shader oebjct is destroyed before it will
// be marked for deletion until shader is detached
void Program::DetachShader(const Shader &shader) {
    glDetachShader(name_, shader.GetName());
    util::Log::Write("Program " , name_ , " detached shader " , shader.GetName());
}

// Link attached shader objects into executable
void Program::Link() {
    glLinkProgram(name_);
    util::Log::Write("Program " , name_ , " linking ");
    LinkStatus();
}

// Check and return status of program linking
// Should be called after Link() method
GLint Program::LinkStatus() {
    GLint status = 0;
    glGetProgramiv(name_, GL_LINK_STATUS, (int *)&status);
    util::Log::Write("Program " , name_ , " link status " , status);
    if (!status) {
        GLint max_length = 0;
        glGetProgramiv(name_, GL_INFO_LOG_LENGTH, &max_length);
        // The max_length includes the NULL character
        std::vector<GLchar> info_log(max_length);
        glGetProgramInfoLog(name_, max_length, &max_length, &info_log[0]);
        util::Log::Write2("Program " , name_ , " linking failed: " , &info_log[0]);
    }
    return status;
}

// Installs a program object as part of current rendering state
void Program::Use() {
    glUseProgram(name_);
}

// Returns the location of an attribute variable
GLint Program::GetAttribLocation(const std::string &attrib_name) const {
    GLint location = glGetAttribLocation(name_, &attrib_name[0]);
    if (location < 0) {
        throw std::runtime_error("(Program) Attribute '" + attrib_name + "' is not active!");
    }
    return location;
}

// Returns the location of an uniform variable
GLint Program::GetUniformLocation(const std::string &uniform_name) const {
    GLint location = glGetUniformLocation(name_, &uniform_name[0]);
    if (location < 0) {
        throw std::runtime_error("(Program) Uniform '" + uniform_name + "' is not active!");
    }
    return location;
}

// Set uniform variables
// TODO: maybe overload functions?
void Program::SetBool(const std::string &name, bool value) const {
    glProgramUniform1i(name_, GetUniformLocation(name), (int)value);
}
void Program::SetInt(const std::string &name, int value) const {
    glProgramUniform1i(name_, GetUniformLocation(name), value);
}
void Program::SetFloat(const std::string &name, float value) const {
    glProgramUniform1f(name_, GetUniformLocation(name), value);
}
void Program::SetVec3(const std::string &name, const glm::vec3 &value) const {
    glProgramUniform3fv(name_, GetUniformLocation(name), 1, &value[0]);
}
void Program::SetVec3(const std::string &name, float x, float y, float z) const {
    glProgramUniform3f(name_, GetUniformLocation(name), x, y, z);
}
void Program::SetMat4(const std::string &name, const glm::mat4 &mat) const {
    glProgramUniformMatrix4fv(name_, GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}


// --------------------------------------------------------------------------------------
// Utility functions (NOT abstraction of opengl calls)
// Compile shaders and link to given program
/*
void Program::CompileShaders(Program &program, const std::string vs_file, const std::string fs_file,
                              const std::string tcs_file, const std::string tes_file,
                              const std::string gs_file) {
    Shader vertex_shader(Shader::ShaderType::kVertexShader);
    Shader tes_cont_shader(Shader::ShaderType::kTessControlShader);
    Shader tes_eval_shader(Shader::ShaderType::kTessEvaluationShader);
    Shader geometry_shader(Shader::ShaderType::kGeometryShader);
    Shader fragment_shader(Shader::ShaderType::kFragmentShader);

    // VERTEX
    if (!vs_file.empty()) {
        vertex_shader.LoadSource(vs_file);
        vertex_shader.Compile();
        program.AttachShader(vertex_shader);
    }

    //TESSALATION CONTROL
    if (!tcs_file.empty()) {
        tes_cont_shader.LoadSource(tcs_file);
        tes_cont_shader.Compile();
        program.AttachShader(tes_cont_shader);
    }

    //TESSALATION EVALUATION
    if (!tes_file.empty()) {
        tes_eval_shader.LoadSource(tes_file);
        tes_eval_shader.Compile();
        program.AttachShader(tes_eval_shader);
    }

    //GEOMETRY
    if (!gs_file.empty()) {
        geometry_shader.LoadSource(gs_file);
        geometry_shader.Compile();
        program.DetachShader(geometry_shader);
    }

    //FRAGMENT
    if (!fs_file.empty()) {
        fragment_shader.LoadSource(fs_file);
        fragment_shader.Compile();
        program.AttachShader(fragment_shader);
    }

    program.Link();

    // Need to be detached or else destructor will never delete shaders
    // Could be auomated with glGetAttachedShaders inside Link() method
    if (!vs_file.empty()) {
        program.DetachShader(vertex_shader);
    }
    if (!tcs_file.empty()) {
        program.DetachShader(tes_cont_shader);
    }
    if (!tes_file.empty()) {
        program.DetachShader(tes_eval_shader);
    }
    if (!gs_file.empty()) {
        program.DetachShader(geometry_shader);
    }
    if (!fs_file.empty()) {
        program.DetachShader(fragment_shader);
    }
}*/
}; //  gl
