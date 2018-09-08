#pragma once
#include <glad/glad.h>
#include <iostream>
#include "buffer.h"

namespace engine::renderer::gl {
class VertexArray {
public:
    VertexArray();
    ~VertexArray();
    // move constructor
    VertexArray(VertexArray&& c) noexcept;
    // move assignment
    VertexArray& operator=(VertexArray&& c) noexcept;
    // Remove possibility to copy object
    VertexArray(VertexArray const&) = delete;
    VertexArray operator=(VertexArray const&) = delete;
    // Return va name (handle)
    GLuint GetName() const;
    // Bind VAO to current context
    void Bind();
    // Unbind VAO
    void Unbind();
    // Tell VAO where to take data from
    void VertexBuffer(GLuint bindingindex, const Buffer &buf, GLintptr offset, GLsizei stride);
    // Tell VAO the source of indices (index array)
    void ElementBuffer(const Buffer & buf);
    // Describe layout and format of data
    void AttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
    // Associate a vertex attribute and a vertex buffer binding for a vertex array object
    void AttribBinding(GLuint attribindex, GLuint bindingindex);
    // Enable a generic vertex attribute array
    void EnableAttrib(GLuint index);
private:
    void Swap(VertexArray& a, VertexArray& b) noexcept;
    // ownership flag
    bool own_ = false;
    // va handle
    GLuint name_ = 0;
};
}; // gl

