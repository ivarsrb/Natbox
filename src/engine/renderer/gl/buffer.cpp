#include "buffer.h"
#include <engine/util/log.h>


namespace engine::renderer::gl {
// Buffers are managed through DSA
Buffer::Buffer() {
    glCreateBuffers(1, &name_);
    own_ = true;
    util::Log::Write("Buffer " , name_ , " created");
}

Buffer::~Buffer() {
    if (own_) {
        util::Log::Write("Buffer ", name_, " destroyed");
        glDeleteBuffers(1, &name_);
    }
}

// Move constructor
Buffer::Buffer(Buffer&& c) noexcept {
    Swap(*this, c);
    util::Log::Write("Buffer move constructed from " , name_ , " to " , c.name_);
}

// Move assignment
Buffer& Buffer::operator=(Buffer&& c) noexcept {
    Swap(*this, c);
    util::Log::Write("Buffer move assigned from " , name_ , " to " , c.name_);
    return *this;
}

GLuint Buffer::GetName() const {
    return name_;
}

// Swap resource handles and other data
void Buffer::Swap(Buffer& a, Buffer& b) noexcept {
    // handle
    std::swap(a.name_, b.name_);
    // ownership
    std::swap(a.own_, b.own_);
}

// Immutable - Allocate memory, pass data (or nullptr) and set usage flags
// Contents of buffer later can be changed but size and type can not
void Buffer::Storage(GLsizeiptr size, const void * data, GLbitfield flags) {
    glNamedBufferStorage(name_, size, data, flags);
    if (data != nullptr) {
        util::Log::Write("Buffer (immutable) " , name_ , " allocated  and filled " , size , " bytes");
    }
    else {
        util::Log::Write("Buffer (immutable) " , name_ , " allocated " , size , " bytes");
    }
}

// Map named buffer range
void * Buffer::MapRange(GLintptr offset, GLsizeiptr length, GLbitfield access) {
    util::Log::Write("Buffer " , name_ , " range mapped, offset " , offset);
    return glMapNamedBufferRange(name_, offset, length, access);
}

// Unmap named buffer
GLboolean Buffer::Unmap() {
    util::Log::Write("Buffer " , name_ , " unmapped");
    return glUnmapNamedBuffer(name_);
}

// Write subdata to named buffer
// Us i to change buffer data at runtime
// Must have set GL_DYNAMIC_STORAGE_BIT in the flags parameter to glNamedBufferStorage()
void Buffer::SubData(GLintptr offset, GLsizeiptr size, const void * data) {
    glNamedBufferSubData(name_, offset, size, data);
    //std::clog << "Subdata written for buffer " << name_ << ", size " << size << ", offset " << offset << std::endl;
}

}; // gl
