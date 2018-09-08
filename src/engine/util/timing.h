#pragma once
#include <chrono>

namespace engine::util {
// Timing for performance measurment
/*
Usage:
clock_.SetTime1();
Operation();
clock_.SetTime2(true);
*/
class Timing {
public:
    Timing();
    void SetTime1();
    void SetTime2(bool show_time);
    void CalcDuration();
    void Reset();
    void Print();
    uint64_t GetDuration();
private:
    std::chrono::steady_clock::time_point t1_;
    std::chrono::steady_clock::time_point t2_;
    uint64_t duration_;
};
};