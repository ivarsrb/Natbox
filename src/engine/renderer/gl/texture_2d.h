#pragma once
#include <glad/glad.h>
#include <iostream>

namespace engine::renderer::gl {
class Texture2D {
public:
    Texture2D();
    ~Texture2D();
    // move constructor
    Texture2D(Texture2D&& c) noexcept;
    // move assignment
    Texture2D& operator=(Texture2D&& c) noexcept;
    // Remove possibility to copy object
    Texture2D(Texture2D const&) = delete;
    Texture2D operator=(Texture2D const&) = delete;
    // Return texture name (handle)
    GLuint GetName() const;

    // Allocate storage for texture data
    void Storage(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
    // Assign image data to texture object
    void SubImage(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels);
    void GenerateMipmap();
    // Bind this texture object to given texture unit
    void BindTextureUnit(GLuint unit);
    // Set texture parameter for default sampler
    void Parameter(GLenum pname, GLint param);
private:
    // Swap object parameters
    void Swap(Texture2D& a, Texture2D& b) noexcept;
    // ownership flag
    bool own_ = false;
    // buffer handle
    GLuint name_ = 0;
};
}; //namespace gl
