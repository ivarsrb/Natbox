#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "input.h"

namespace engine::platform {
class IPlatformApp {
public:
    // To pass initial application configuration
    struct Configuration {
        glm::ivec2 win_size;
        std::string win_title; 
        glm::ivec2 win_pos;
        // Weather gui is initialised upon initializing
        bool use_gui;
    };
    // Application time data
    struct Timing {
        // Time since beginning of application
        double elapsed;
        // Time it took to render last frame
        double delta;
        // Used to store value for delta calculation
        double last_frame;
    };
    IPlatformApp(const Configuration& config);
    virtual ~IPlatformApp();
    void Run();
protected:
    // Window size
    glm::ivec2 window_size_;
    bool use_gui_;
private:
    virtual void Update(const Timing *time, const Input *input) = 0;
    virtual void Render(const Timing *time) = 0;
    virtual void Resize(const glm::ivec2 &size) = 0;
    virtual void KeyPress(const Input *input) = 0;
    virtual void MouseMove(const Input *input) = 0;
    virtual void MouseButtonPress(const Input *input) = 0;
    virtual void MouseScroll(const Input *input) = 0;
    static void ResizeCallback(GLFWwindow* window, int width, int height);
    static void ErrorCallback(int32_t error, const char* description);
    static void KeyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void CreateContext();
    void DestroyContext();
    void LoadOpenGL();
    // Context window
    GLFWwindow* context_;
    // Window title
    std::string title_;
    // Time values for application
    Timing time_;
    // Input data
    Input input_;
};
}; // platform
