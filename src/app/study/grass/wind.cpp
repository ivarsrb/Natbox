#include "wind.h"
#include <imgui/imgui.h>
#include <engine/util/log.h>

using namespace engine;

namespace app::study::grass {
Wind::Wind() {
    Reset();
}

void Wind::Reset() {
    // Initial direction and strenth in cartesian coordinates system
    tp::Vec3 c_initial_vector_ = tp::Vec3(1.0, 0.0, 0.0);
    // Transform to spherical
    s_vector_.InitFromCartesian(c_initial_vector_); 
}

void Wind::Stop() {
    SetSpeed(0);
}

void Wind::Update(engine::tp::Real dt) {

}

// In spherical coordinates speed is represented by radius 
void Wind::SetSpeed(engine::tp::Real speed) {
    if (speed < 0) {
        speed = 0.0;
        util::Log::Write2("Wind speed is negative; set to 0");
    }    
    s_vector_.radius = speed;
}

// Rotation angle theta of wind vector in spherical coordinates
void Wind::SetRotation(engine::tp::Real angle) {
    s_vector_.theta = angle;
}

// Curent wind vector in cartesian space
tp::Vec3 Wind::CartesianVector() const {
    return s_vector_.GetCartesian();
}

// Render UI for tests and debugging
// NOTE: actual rendering takes place after Render function, this only sets up what to render and how it reacts 
void Wind::RenderGui() {
    ImGui::Begin("Wind");
    tp::Vec3 c_vector = CartesianVector();
    ImGui::Text("Vector: x  %0.2f, y %0.2f, z %0.2f", c_vector.x, c_vector.y, c_vector.z);
    float wind_speed = (float)s_vector_.radius;
    ImGui::SliderFloat("Speed ", &wind_speed, 0.0f, 5.0f);
    SetSpeed(wind_speed);
    float rot_angle = (float)s_vector_.theta;
    ImGui::SliderFloat("Rotation (rad)", &rot_angle, 0.0f, 2*M_PI);
    SetRotation(rot_angle);
    if (ImGui::Button(" Reset ")) {
       Reset();
    }
    ImGui::SameLine();
    if (ImGui::Button(" Stop ")) {
       Stop();
    }
    
    ImGui::End();
}
};
