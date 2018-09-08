#include "timing.h"
#include <iostream>

namespace engine::util {
Timing::Timing() {
    Reset();
}

void Timing::SetTime1() {
    Reset();
    t1_ = std::chrono::steady_clock::now();
}

void Timing::SetTime2(bool show_time) {
    t2_ = std::chrono::steady_clock::now();
    CalcDuration();
    if (show_time) {
        Print();
    }
}

void Timing::Reset() {
    t1_ = t2_ = {};
    duration_ = 0;
}

void Timing::CalcDuration() {
    duration_ = std::chrono::duration_cast<std::chrono::milliseconds>(t2_ - t1_).count();
}

void Timing::Print() {
    std::cout << "miliseconds - " << duration_ << std::endl;
}
// Miliseconds
uint64_t Timing::GetDuration() {
    return duration_;
}
};