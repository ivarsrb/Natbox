#include "frame_buffer.h"
#include <engine/util/log.h>


namespace engine::renderer::gl {
FrameBuffer::FrameBuffer() {
    glCreateFramebuffers(1, &name_);
    own_ = true;
    util::Log::Write("Framebuffer " , name_ , " created");
}

FrameBuffer::~FrameBuffer() {
    if (own_) {
        util::Log::Write("Framebuffer ", name_, " destroyed");
        glDeleteFramebuffers(1, &name_);
    }
}

// Move constructor
FrameBuffer::FrameBuffer(FrameBuffer&& c) noexcept {
    Swap(*this, c);
    util::Log::Write("Framebuffer move constructed from " , name_ , " to " , c.name_);
}

// Move assignment
FrameBuffer& FrameBuffer::operator=(FrameBuffer&& c) noexcept {
    Swap(*this, c);
    util::Log::Write("Framebuffer move assigned from " , name_ , " to " , c.name_);
    return *this;
}

GLuint FrameBuffer::GetName() const {
    return name_;
}

// Swap resource handles and other data
void FrameBuffer::Swap(FrameBuffer& a, FrameBuffer& b) noexcept {
    // handle
    std::swap(a.name_, b.name_);
    // ownership
    std::swap(a.own_, b.own_);
}

// Bind current framebuffer to  read and write target.
// All consecutive draw commands renders to this framebuffer
void FrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, name_);
    //util::Log::Write("Freamebuffer " , name_ , " bound to read/write framebuffer");
}

// Set default (system) framebuffer as current one so all commands render to screen
// static
void FrameBuffer::BindToDefault() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //util::Log::Write("Freamebuffer " , name_ , " unbound");
}

// Check for framebuffer completeness
bool FrameBuffer::CheckStatusComplete() {
    bool completeness;
    GLenum status = glCheckNamedFramebufferStatus(name_, GL_FRAMEBUFFER);
    completeness = (status == GL_FRAMEBUFFER_COMPLETE);
    if (completeness) {
        util::Log::Write("Freamebuffer " , name_ , " is complete");
    }
    else {
        util::Log::Write2("Freamebuffer " , name_ , " is NOT complete");
    }
    return completeness;
}

// Set texture as specified attachment
void FrameBuffer::Texture(const Texture2D & texture, GLenum attachment) {
    glNamedFramebufferTexture(name_, attachment, texture.GetName(), 0);
    util::Log::Write("Texture  " , texture.GetName() , " attached to framebuffer " , name_);
}

// Specify which color buffers are to be drawn into, 0 - in this case
// NOTE: GL_COLOR_ATTACHMENT_0 could be set by default
void FrameBuffer::DrawBuffer() {
    glNamedFramebufferDrawBuffer(name_, GL_COLOR_ATTACHMENT0);
}



}; // gl
