#include "timer.h"
#include <iostream>

namespace engine::platform {
Timer::Timer(tp::Real fixed_dt): real_time_(0),real_dt_(0),virtual_time_(0),virtual_dt_(fixed_dt), virtual_accumulator_(0) {
}
// Should be called right before frame loop
void Timer::Reset() {
    real_dt_ = 0;
    virtual_time_ = 0;
    virtual_accumulator_ = 0;
    // null the timer so glfwGetTime() works properly for us (otherwise it takes time from beginning of initialization)
    glfwSetTime(0);
    real_time_ = glfwGetTime();
}
// Should be called before simulation and rendering
void Timer::Update() {
    tp::Real new_time = glfwGetTime();
    real_dt_ = new_time - real_time_;
    real_time_ = new_time;
    virtual_accumulator_ += real_dt_;
    // For statistic
    //std::cout << updates_per_frame << std::endl;
    updates_per_frame = 0;
}
// Ised to check if simulation update is going to happen
bool Timer::ShouldUpdateVirtual() const {
    return (virtual_accumulator_ >= virtual_dt_);
}
// Should be called inside simulation subloop at the end of it
void Timer::UpdateVirtual() {
    virtual_accumulator_ -= virtual_dt_;
    virtual_time_ += virtual_dt_;
    // For statistic
    ++updates_per_frame;
}
}; // platform