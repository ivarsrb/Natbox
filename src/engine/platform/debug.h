#pragma once
#include <iostream>
#include <glad/glad.h>

namespace engine::platform {
class Debug {
public:
    // Set up Opengl 4.2+ debug output
    static void Init();
    static void APIENTRY GLDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar *message, const void *userParam);
};

}; // platform


