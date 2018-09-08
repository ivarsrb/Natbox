#include "texture_2d.h"
#include <engine/renderer/image.h>
#include <engine/util/log.h>

namespace engine::renderer::gl {
Texture2D::Texture2D() {
    glCreateTextures(GL_TEXTURE_2D, 1, &name_);
    own_ = true;
    util::Log::Write("Texture2D " , name_ , " created");
}

Texture2D::~Texture2D() {
    if (own_) {
        util::Log::Write("Texture2D " , name_ , " destroyed");
        glDeleteTextures(1, &name_);
    }
}

// Move constructor
Texture2D::Texture2D(Texture2D&& c) noexcept {
    Swap(*this, c);
    util::Log::Write("Texture2D move constructed from " , name_ , " to " , c.name_);
}

// Move assignment
Texture2D& Texture2D::operator=(Texture2D&& c) noexcept {
    Swap(*this, c);
    util::Log::Write("Texture2D move assigned from " , name_ , " to " , c.name_);
    return *this;
}

GLuint Texture2D::GetName() const {
    return name_;
}

// Swap resource handles and other data
void Texture2D::Swap(Texture2D& a, Texture2D& b) noexcept {
    // handle
    std::swap(a.name_, b.name_);
    // ownership
    std::swap(a.own_, b.own_);
}

// Allocate storage for texture data
void Texture2D::Storage(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) {
    glTextureStorage2D(name_, levels, internalformat, width, height);
    util::Log::Write("Texture2D " , name_ , " storage allocated. Dim: " , width , "x" , height);
}

// Assign image data to texture object
void Texture2D::SubImage(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) {
    glTextureSubImage2D(name_, level, xoffset, yoffset, width, height, format, type, pixels);
    util::Log::Write("Texture2D " , name_ , " image added");
}

void Texture2D::GenerateMipmap() {
    glGenerateTextureMipmap(name_);
    util::Log::Write("Texture2D " , name_ , " mipmap generated");
}

// Bind this texture object to given texture unit
// In shader specifie unit number with - layout(binding = 0)
void Texture2D::BindTextureUnit(GLuint unit) {
     glBindTextureUnit(unit, name_);
 // Log::Write("Texture2D " , name_ , " bound to texture unit " , unit);
}

// Set given binding unit to default
// NOTE: This function normally should be static because it doesnt belong to our texture specifically
/*
void Texture2D::UnbindTextureUnit(GLuint unit) {
  glBindTextureUnit(unit, 0);
  std::clog << "Texture2D " << name_ << " UNbound from texture unit " << unit << std::endl;
}
*/

// Set texture parameter for default sampler
void Texture2D::Parameter(GLenum pname, GLint param) {
    glTextureParameteri(name_, pname, param);
    //util::Log::Write("Texture2D " , name_ , " parameter " , pname , " set to " , param);
}


}; //namespace gl
