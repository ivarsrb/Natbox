#pragma once
#include <glm/glm.hpp>
#include <engine/core/types.h>

// Camera system using Euler angles
namespace engine::core {
class Camera {
public:
    enum Movement {
        kForward,
        kBackward,
        kLeft,
        kRight,
        kUp,
        kDown
    };
    Camera(glm::tvec3<tp::Real> pos, tp::Real yaw, tp::Real pitch);

    glm::mat4 GetViewMatrix() const;
    void ProcessKeyboard(Movement direction, tp::Real delta_time);
    void ProcessMouseMovement(tp::Real xoffset, tp::Real yoffset, bool constrain_pitch = true);
    void ProcessMouseScroll(tp::Real yoffset);
    void SetSpeed(tp::Real speed);
    // Get field of view (zoom)
    tp::Real GetFOV() const;
    glm::tvec3<tp::Real> GetPosition() const;
    glm::tvec3<tp::Real> GetFront() const;
    tp::Real GetSpeed() const;

private:
    // Movement speed
    static constexpr tp::Real kSpeed = 1.4f; // m/s - 1.4 = human walking speed
    // Freelook sensitivity
    static constexpr tp::Real kSensitivity = 0.25f;
    // Field of view in degrees
    static constexpr tp::Real kZoom = 45.0f;
    void UpdateVectors();
    // Camera Attributes
    // Position of eyes
    glm::tvec3<tp::Real> position_;
    // Vector of view direction
    glm::tvec3<tp::Real> front_;
    // Vector upside up
    glm::tvec3<tp::Real> up_;
    // Vector to the right of eyes
    glm::tvec3<tp::Real> right_;
    // World orientation
    glm::tvec3<tp::Real> world_up_;
    // Values during actual calculation
    // Eular Angles
    tp::Real yaw_;
    tp::Real pitch_;
    // Camera options
    tp::Real movement_speed_;
    tp::Real mouse_sensitivity_;
    // Field of view
    tp::Real zoom_;
};
}