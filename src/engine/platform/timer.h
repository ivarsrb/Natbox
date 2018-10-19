#pragma once
#include <GLFW/glfw3.h>
#include <engine/core/types.h>

namespace engine::platform {
// Manages and keeps track of app time for simulation and rendering
class Timer {
public:
    Timer(tp::Real fixed_dt);
    // Should be called right before frame loop
    void Reset();
    // Should be called before simulation and rendering
    void Update();
    // Ised to check if simulation update is going to happen
    bool ShouldUpdateVirtual() const;
    // Should be called inside simulation subloop at the end of it
    void UpdateVirtual();
    tp::Real RealTime() const {return real_time_;};
    tp::Real RealDt() const {return real_dt_;};
    tp::Real VirtualTime() const {return virtual_time_;};
    tp::Real VirtualDt() const {return virtual_dt_;};
    tp::Real VirtualAccum() const {return virtual_accumulator_;};
    // For statistics
    // How many time update functions is called in this iteration of rendering
    uint32_t updates_per_frame;
private:
    // Real world:
    // Real time that has ellapsed since start of the program
    // This value keeps updating every frame during whole run of the application
    tp::Real real_time_;
    // Time it took to render last frame, this value is varying depending on framerate (not fixed) 
    //  and should not be used in any critical simulations
    tp::Real real_dt_;
    // Simulation world:
    // Virtual time that has passed since simulation begun.
    // Should be used for actual simulations.
    // This time stops if simulation is paused while real_time does not.
    tp::Real virtual_time_;
    // Fixed time interval between two simulation calls.
    // Should be used in simulation instead of real_dt.
    tp::Real virtual_dt_;
    // Used to regulate how many simulations per frame should be performed
    tp::Real virtual_accumulator_;
};
}; // platform