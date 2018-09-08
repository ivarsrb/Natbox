#pragma once
#include <glad/glad.h>
#include <iostream>
#include "texture_2d.h"

namespace engine::renderer::gl {
class FrameBuffer {
public:
    FrameBuffer();
    ~FrameBuffer();
    // move constructor
    FrameBuffer(FrameBuffer&& c) noexcept;
    // move assignment
    FrameBuffer& operator=(FrameBuffer&& c) noexcept;
    // Remove possibility to copy object
    FrameBuffer(FrameBuffer const&) = delete;
    FrameBuffer operator=(FrameBuffer const&) = delete;
    // Return buffer name (handle)
    GLuint GetName() const;
    void Bind();
    static void BindToDefault();
    bool CheckStatusComplete();
    void Texture(const Texture2D &texture, GLenum attachment);
    void DrawBuffer();
private:
    // Swap object parameters
    void Swap(FrameBuffer& a, FrameBuffer& b) noexcept;
    // ownership flag
    bool own_ = false;
    // frame buffer handle
    GLuint name_ = 0;
};
}; // gl


