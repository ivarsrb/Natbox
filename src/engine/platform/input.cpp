#include "input.h"
#include <algorithm>

namespace engine::platform {
Input::Input(bool cursor_disabled) {
    std::fill_n(keyboard.active, 1024, false);
    keyboard.key = 0;
    keyboard.action = 0;
    mouse.pos = glm::vec2(0.0f);
    mouse.scroll_offset = glm::vec2(0.0f);
    mouse.last_position = glm::vec2(0.0f);
    mouse.offset = glm::vec2(0.0f);
    mouse.cursor_disabled = cursor_disabled;
    mouse.sensitivity = 0.05f;
    mouse.first_move = true;
}

void Input::UpdateMouse(float xpos, float ypos) {
    mouse.pos = glm::vec2(xpos, ypos);
    // When mouse input is calculated for free look
    if (mouse.cursor_disabled) {
        // When mouse is touched for the first time make sure it does not jump
        if (mouse.first_move) {
            mouse.last_position = glm::vec2(xpos, ypos);
            mouse.first_move = false;
        }
        // Calculate mouse data when moving
        mouse.offset.x = xpos - mouse.last_position.x;
        // Reversed since y-coordinates range from bottom to top
        mouse.offset.y = mouse.last_position.y - ypos;
        mouse.last_position.x = xpos;
        mouse.last_position.y = ypos;
        mouse.offset *= mouse.sensitivity;
    }
}
void Input::UpdateKeys(int32_t key, int32_t action) {
    keyboard.key = key;
    keyboard.action = action;
    // Keys that are currently being held by user, used outside keypress function
    // Usually in keypress function single hit or release is beinged checked
    if (key >= 0 && key < keyboard.active_keys_max) {
        if (action == kPress) {
            keyboard.active[key] = true;
        }
        else if (action == kRelease) {
            keyboard.active[key] = false;
        }
    }
}

void Input::UpdateMouseButton(int button, int action, int mods) {
    mouse_button.button = button;
    mouse_button.action = action;
    mouse_button.mods = mods;
}

void Input::UpdateScroll(float xoffset, float yoffset) {
    if (mouse.cursor_disabled) {
        mouse.scroll_offset = glm::vec2(xoffset, yoffset);
    }
}
};// platform