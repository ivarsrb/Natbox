#include "vertex_array.h"
#include <engine/util/log.h>

namespace engine::renderer::gl {
VertexArray::VertexArray() {
    glCreateVertexArrays(1, &name_);
    own_ = true;
    util::Log::Write("VAO " , name_ , " created");
}

VertexArray::~VertexArray() {
    if (own_) {
        util::Log::Write("VAO " , name_ , " destroyed");
        glDeleteVertexArrays(1, &name_);
    }
}

// move constructor
VertexArray::VertexArray(VertexArray && c) noexcept {
    Swap(*this, c);
    util::Log::Write("VAO move constructed from " , name_ , " to " , c.name_);
}

// move assignment
VertexArray& VertexArray::operator=(VertexArray && c) noexcept {
    Swap(*this, c);
    util::Log::Write("VAO move assigned from " , name_ , " to " , c.name_);
    return *this;
}

GLuint VertexArray::GetName() const {
    return name_;
}

void VertexArray::Swap(VertexArray & a, VertexArray & b) noexcept  {
    std::swap(a.name_, b.name_);
    std::swap(a.own_, b.own_);
}

// Bind VAO to current context
void VertexArray::Bind() {
    glBindVertexArray(name_);
    //std::clog << "VAO " << name_ << " bound to context" << std::endl;
}

// Unbind this VAO from context
void VertexArray::Unbind() {
    glBindVertexArray(0);
    //std::clog << "VAO " << name_ << " unbound" << std::endl;
}


// Tell VAO where to take data from (what buffer to use)
void VertexArray::VertexBuffer(GLuint bindingindex, const Buffer &buf, GLintptr offset, GLsizei stride) {
    glVertexArrayVertexBuffer(name_, bindingindex, buf.GetName(), offset, stride);
    util::Log::Write("Buffer " , buf.GetName() , " bound to VAO " , name_ , ", bind index " , bindingindex);
}

// Tell VAO the source of indices (index array)
void VertexArray::ElementBuffer(const Buffer &buf) {
    glVertexArrayElementBuffer(name_, buf.GetName());
    util::Log::Write("Element buffer " , buf.GetName() , " bound to VAO " , name_);
}

// Describe layout and format of data
void VertexArray::AttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) {
    glVertexArrayAttribFormat(name_, attribindex, size, type, normalized, relativeoffset);
    util::Log::Write("VAO " , name_ , " attribute format set");
}

// Associate a vertex attribute and a vertex buffer binding for a vertex array object
void VertexArray::AttribBinding(GLuint attribindex, GLuint bindingindex) {
    glVertexArrayAttribBinding(name_, attribindex, bindingindex);
    util::Log::Write("VAO " , name_ , " attribute bound attr. index " , attribindex , ", binding index " , bindingindex);
}

// Enable a generic vertex attribute array
void VertexArray::EnableAttrib(GLuint index) {
    glEnableVertexArrayAttrib(name_, index);
    util::Log::Write("VAO " , name_ , " attribute array enabled, index " , index);
}
}; // gl