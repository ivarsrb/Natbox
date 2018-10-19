#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "input.h"
#include "timer.h"

namespace engine::platform {
class IPlatformApp {
public:
    // To pass initial application configuration
    struct Configuration {
        glm::ivec2 win_size;
        std::string win_title; 
        glm::ivec2 win_pos;
        // Weather gui is initialised upon initializing
        bool use_gui = true;
        // Weather frame rate is fixed to monitor refresh rate
        bool fixed_fps = true;
        // What should the fixed interval between simulation updates in seconds
        tp::Real fixed_time_step = (1 / 60.);
    };
    IPlatformApp(const Configuration& config);
    virtual ~IPlatformApp();
    void Run();
protected:
    Configuration configuration;
private:
    virtual void Update(const Timer &timer, const Input &input) = 0;
    virtual void Render() = 0;
    virtual void RenderGUI(const Timer &timer) = 0;
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
    Input input_;
    Timer timer_;
};
}; // platform
