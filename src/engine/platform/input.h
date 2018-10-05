#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace engine::platform {
class Input {
public:
    // Keys
    enum Key : int32_t {
        kA = GLFW_KEY_A,
        kD = GLFW_KEY_D,
        kS = GLFW_KEY_S,
        kW = GLFW_KEY_W,
        kU = GLFW_KEY_U,
        kI = GLFW_KEY_I,
        kO = GLFW_KEY_O,
        kJ = GLFW_KEY_J,
        kK = GLFW_KEY_K,
        kL = GLFW_KEY_L,
        kR = GLFW_KEY_R,
        kF = GLFW_KEY_F,
        kUp = GLFW_KEY_UP,
        kDown = GLFW_KEY_DOWN,
        kLeft = GLFW_KEY_LEFT,
        kRight = GLFW_KEY_RIGHT,
        kPageUp = GLFW_KEY_PAGE_UP,
        kPageDown = GLFW_KEY_PAGE_DOWN,
    };
    // Action of key ()mous or keyboard
    enum Action : int32_t {
        kPress = GLFW_PRESS,
        kRelease = GLFW_RELEASE
    };
    // Mouse buttons
    enum MouseButton : int32_t {
        kLeftButton = GLFW_MOUSE_BUTTON_LEFT,
        kRightButton = GLFW_MOUSE_BUTTON_RIGHT
    };

    struct MouseData {
        // Current x,y
        glm::vec2 pos;
        // Current mouse scrolling offset
        glm::vec2 scroll_offset;
        // To calculate offset of mouse movement
        glm::vec2 last_position;
        // How much mouse is moved
        glm::vec2 offset;
        // When enabled, mouse data isn't calculated
        bool cursor_disabled;
        // Mouse movement sensitivity
        float sensitivity;
        // To determine first mouse touch
        bool first_move;
    };
    struct KeyData {
        // current data
        int32_t key;
        int32_t action;
        // Keys that are currently being held by user, usually used outside keypress function
        static constexpr int32_t active_keys_max = 1024;
        bool active[active_keys_max];
    };
    struct MouseButtonData {
        int32_t button;
        int32_t action;
        int32_t mods;
    };

    Input(bool cursor_disabled);
    void UpdateMouse(float xpos, float ypos);
    void UpdateKeys(int32_t key, int32_t action);
    void UpdateMouseButton(int button, int action, int mods);
    void UpdateScroll(float xoffset, float yoffset);
    MouseData mouse;
    KeyData keyboard;
    MouseButtonData mouse_button;
};
}; // platform