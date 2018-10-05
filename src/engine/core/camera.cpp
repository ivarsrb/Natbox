#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace engine::core {
Camera::Camera(glm::tvec3<tp::Real> pos, tp::Real yaw, tp::Real pitch)
                  : movement_speed_(kSpeed),
                    mouse_sensitivity_(kSensitivity), 
                    zoom_(kZoom) {
    position_ = pos;
    world_up_ = glm::tvec3<tp::Real>(0.0, 1.0, 0.0),
    yaw_ = yaw;
    pitch_ = pitch;

    UpdateVectors();
}

 glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position_, position_ + front_, up_);
}

 // Camera movement processed with key-presses
void Camera::ProcessKeyboard(Movement direction, tp::Real delta_time) {
    tp::Real velocity = movement_speed_ * delta_time;
    switch (direction) {
    case kForward:
        position_ += front_ * velocity;
        break;
    case kBackward:
        position_ -= front_ * velocity;
        break;
    case kLeft:
        position_ -= right_ * velocity;
        break;
    case kRight:
        position_ += right_ * velocity;
        break;
    case kUp:
        position_ += up_ * velocity;
        break;
    case kDown:
        position_ -= up_ * velocity;
        break;   
    }
}

// Camera free-look from mouse movement
// constrain_pitch - weather free look is constrained
void Camera::ProcessMouseMovement(tp::Real xoffset, tp::Real yoffset, bool constrain_pitch) {
    xoffset *= mouse_sensitivity_;
    yoffset *= mouse_sensitivity_;

    // Make sure yaw_ stays between 0 and 360
    yaw_ = glm::mod(yaw_ + xoffset, (tp::Real)360.0);
    pitch_ += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrain_pitch)
    {
        constexpr tp::Real kConstraint = 89.0;
        if (pitch_ > kConstraint)
            pitch_ = kConstraint;
        if (pitch_ < -kConstraint)
            pitch_ = -kConstraint;
    }

    UpdateVectors();
}

// Vertical mouse scroll processing
void Camera::ProcessMouseScroll(tp::Real yoffset) {
    // Calculate field of view
    constexpr tp::Real kMinZoom = 1.0;
    if (zoom_ >= kMinZoom && zoom_ <= kZoom)
        zoom_ -= yoffset;
    if (zoom_ <= kMinZoom)
        zoom_ = kMinZoom;
    if (zoom_ >= kZoom)
        zoom_ = kZoom;
}

void Camera::SetSpeed(tp::Real speed) {
    movement_speed_ = speed;
}

// Get field of view (zoom)
tp::Real Camera::GetFOV() const {
    return zoom_;
}

glm::tvec3<tp::Real> Camera::GetPosition() const {
    return position_;
}

glm::tvec3<tp::Real> Camera::GetFront() const {
    return front_;
}

tp::Real Camera::GetSpeed() const {
    return movement_speed_;
}

void Camera::UpdateVectors() {
    // Calculate the new Front vector
    glm::tvec3<tp::Real> front;
    //front.x = cos(glm::radians(pitch_))  * cos(glm::radians(yaw_));
    //front.y = sin(glm::radians(pitch_));
    //front.z = cos(glm::radians(pitch_)) * sin(glm::radians(yaw_));
    front.x = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = -cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
}
}; // core