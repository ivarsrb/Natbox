#pragma once
/* #ifdef __gl_h_
#error DEFFFFFFINED GL 
#endif

#ifdef __glad_h_
#error DEFFFFFFINED GLAD 
#endif  */

#include <glad/glad.h>
#include <iostream>

namespace engine::renderer::gl {
class Buffer {
public:
    Buffer();
    ~Buffer();
    // move constructor
    Buffer(Buffer&& c) noexcept;
    // move assignment
    Buffer& operator=(Buffer&& c) noexcept;
    // Remove possibility to copy object
    Buffer(Buffer const&) = delete;
    Buffer operator=(Buffer const&) = delete;
    // Return buffer name (handle)
    GLuint GetName() const;
    // Allocate immutable storage for buffer data, pass data (or nullptr) and set usage flags
    void Storage(GLsizeiptr size, const void* data, GLbitfield flags);
    // Map named buffer range (preferred over Map())
    void *MapRange(GLintptr offset, GLsizeiptr length, GLbitfield access);
    // Unmap named buffer
    GLboolean Unmap();
    // Write subdata to named buffer
    void SubData(GLintptr offset, GLsizeiptr size, const void * data);
private:
    // Swap object parameters
    void Swap(Buffer& a, Buffer& b) noexcept;
    // ownership flag
    bool own_ = false;
    // buffer handle
    GLuint name_ = 0;
};
}; // gl


