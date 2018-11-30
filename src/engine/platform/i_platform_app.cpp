#include "i_platform_app.h"
#include <exception>
#include <iostream>
#include "debug.h"
#include <imgui/imgui.h>
#include "gui/imgui_impl_glfw_gl3.h"

namespace engine::platform {
IPlatformApp::IPlatformApp(const Configuration& config)
                             : configuration{ config },
                               input_(true),
                               timer_(configuration.fixed_time_step)
{
    //time_ = { 0.0, 0.0, 0.0 };
    // Set up platform
    glfwSetErrorCallback(ErrorCallback);
    if (!glfwInit()) {
        throw std::runtime_error("(Platform) could not initialize GLFW");
    }
    // Clears previous windows hints
    glfwDefaultWindowHints();
    const uint32_t opengl_vers_maj = 4;
    const uint32_t opengl_vers_min = 5;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_vers_maj);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_vers_min);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
    // sets OepnGL debug context so that debug callback for opengl functions reports errors (this is not logging)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
    // For fullscreen only
    //glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
    /*
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    */
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    
    CreateContext();
    // Set window pointer to use current object in callback function
    glfwSetWindowUserPointer(context_, this);
    // Callbacks
    glfwSetFramebufferSizeCallback(context_, ResizeCallback);
    glfwSetKeyCallback(context_, KeyCallback);
    glfwSetCursorPosCallback(context_, MouseCallback);
    glfwSetMouseButtonCallback(context_, MouseButtonCallback);
    glfwSetScrollCallback(context_, ScrollCallback);
    
    // v-sync (if something wrong put after opengl loading)
    // NOTE: possible problems on ATI drivers
    glfwSwapInterval((int)configuration.fixed_fps);
    // When cursor disabled it does not leave window and is not visible
    glfwSetInputMode(context_, GLFW_CURSOR, (input_.mouse.cursor_disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
    glfwSetWindowPos(context_, config.win_pos.x, config.win_pos.y);
    LoadOpenGL();
    // Sets up debug output for OpenGL calls if project Debug configuration 
    Debug::Init();
    // GUI
    if (configuration.use_gui) {
        // Setup ImGui binding
        ImGui_ImplGlfwGL3_Init(context_, false /* set or not set callbacks */);
        // To be able to enter text
        glfwSetCharCallback(context_, ImGui_ImplGlfwGL3_CharCallback);
        // Setup style
        ImGui::StyleColorsClassic();
        //ImGui::StyleColorsDark();
    }
}

IPlatformApp::~IPlatformApp() {
    if (configuration.use_gui) {
        ImGui_ImplGlfwGL3_Shutdown();
    }
    DestroyContext();
    glfwTerminate();
}

// Application main loop
// TODO: intewrpolation should take place in rendering current state so animations appear smoother
void IPlatformApp::Run() {
    timer_.Reset();
    while (!glfwWindowShouldClose(context_)) {
        glfwPollEvents();
        timer_.Update();
        while (timer_.ShouldUpdateVirtual()) {
            // Update is made with fixed timestep (compared to render with varying)
            // NOTE: Sometimes update skips, sometimes is called mutiple times in loop compared to Render()
            Update(timer_, input_);
            timer_.UpdateVirtual();
        }
        Render();
        if (configuration.use_gui) {
            ImGui_ImplGlfwGL3_NewFrame();
            RenderGUI(timer_);
            ImGui::Render();
        }
        glfwSwapBuffers(context_);
    }
}

// Create window/context
void IPlatformApp::CreateContext() {
    context_ = glfwCreateWindow(configuration.win_size.x, configuration.win_size.y, configuration.win_title.c_str(), nullptr, nullptr);
    if (!context_) {
        glfwTerminate();
        throw std::runtime_error("(Platform) could not create context");
    }
    glfwMakeContextCurrent(context_);
}

void IPlatformApp::DestroyContext() {
    glfwDestroyWindow(context_);
}

// Load OpenGL functions
void IPlatformApp::LoadOpenGL() {
    // Load functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("(Platform) Failed to initialize GLAD");
    }
}

// Resize screen 
void IPlatformApp::ResizeCallback(GLFWwindow * window, int width, int height) {
    // Retrieve current window pointer to acess member variables
    IPlatformApp* this_obj = static_cast<IPlatformApp*>(glfwGetWindowUserPointer(window));
    this_obj->Resize(glm::ivec2(width, height)); //virtual
}

//static
void IPlatformApp::ErrorCallback(int32_t error, const char * description) {
    std::cerr << "Platform manager error: " << description << std::endl;
}

void IPlatformApp::KeyCallback(GLFWwindow * window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
    // Retrieve current window pointer to acess member variables
    IPlatformApp* this_obj = static_cast<IPlatformApp*>(glfwGetWindowUserPointer(window));
    // "Built in" key cheking
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    // Enable fullscreen mode
    if (key == GLFW_KEY_F9 && action == GLFW_PRESS) {
        // TODO maybe use fullscreen windowed mode not full fullscreen mode
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        // This doesnt work with some ATI drivers (like Windows but fullscreen sets full fps anyway). With linux we need this
        glfwSwapInterval((int)this_obj->configuration.fixed_fps);
        glfwSetWindowMonitor(this_obj->context_, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
   
    // If Imgui wants input give it to them
    ImGuiIO& io = ImGui::GetIO();
    // NOTE: Problem with pressing enter and no being able to set focus later happens because
    // when we press enter io.WantCaptureKeyboard and io.WantTextInput are both 0 and imgui keycallbakc is not called, 
    // that's why we also check if ENTER was pressed here 
    if (this_obj->configuration.use_gui && (io.WantCaptureKeyboard || io.WantTextInput || key == GLFW_KEY_ENTER)) {
       ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
    }
    else {
        this_obj->input_.UpdateKeys(key, action);
        this_obj->KeyPress(this_obj->input_); //virtual
    }
    
}

void IPlatformApp::MouseCallback(GLFWwindow * window, double xpos, double ypos) {
    IPlatformApp* this_obj = static_cast<IPlatformApp*>(glfwGetWindowUserPointer(window));
    this_obj->input_.UpdateMouse((float)xpos, (float)ypos);
    this_obj->MouseMove(this_obj->input_); //virtual

}

void IPlatformApp::MouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
    IPlatformApp* this_obj = static_cast<IPlatformApp*>(glfwGetWindowUserPointer(window));

    // Enable/disable mouse cursor
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        this_obj->input_.mouse.cursor_disabled = !this_obj->input_.mouse.cursor_disabled;
        glfwSetInputMode(this_obj->context_, GLFW_CURSOR, (this_obj->input_.mouse.cursor_disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
        // Make sure camera dont jump when returning to camera controle mode
        if (this_obj->input_.mouse.cursor_disabled) {
            this_obj->input_.mouse.first_move = true;
        }
    }

    // If Imgui wants input give it to them
    ImGuiIO& io = ImGui::GetIO();
    if (this_obj->configuration.use_gui && !this_obj->input_.mouse.cursor_disabled && io.WantCaptureMouse) {
        ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);
    }
    else {
        this_obj->input_.UpdateMouseButton(button, action, mods);
        this_obj->MouseButtonPress(this_obj->input_); //virtual
    }
}

void IPlatformApp::ScrollCallback(GLFWwindow * window, double xoffset, double yoffset) {
    IPlatformApp* this_obj = static_cast<IPlatformApp*>(glfwGetWindowUserPointer(window));
    //ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);
    this_obj->input_.UpdateScroll((float)xoffset, (float)yoffset);
    this_obj->MouseScroll(this_obj->input_); //virtual
}
}; // platform