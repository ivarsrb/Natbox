#pragma once
#include <iostream>

namespace engine::util {
class Log {
public:
    // Stream where the log is written
    enum Type {
        kClog,
        kFile, // not implemented
    };
    // Severity of logged information
    enum Severity {
        kAll = 0,
        kAttention = 1,
        kError = 2,
    };


    // Write data to output 
    // NOTE: templates cannot be defined in cpp file
    // Write[n] - n - log detail level
    template<typename T>
    inline static void Write(T &&t) {
    if (enabled && output == kClog && detail == kAll) {
        std::clog << t << "\n";   
    }
    }
    template<typename T>
    inline static void Write1(T &&t) {
    if (enabled && output == kClog && detail <= kAttention) {
        std::clog << t << "\n";
    }
    }
    template<typename T>
    inline static void Write2(T &&t) {
    if (enabled && output == kClog && detail <= kError) {
        std::clog << t << "\n";
    }
    }

    template<typename Head, typename... Tail>
    inline static void Write(Head &&head, Tail&&... tail) {
    if (enabled && output == kClog && detail == kAll) {
        std::clog << head;
        Write(std::forward<Tail>(tail)...);
    }
    }
    template<typename Head, typename... Tail>
    inline static void Write1(Head &&head, Tail&&... tail) {
    if (enabled && output == kClog && detail <= kAttention) {
        std::clog << head;
        Write1(std::forward<Tail>(tail)...);
    }
    }
    template<typename Head, typename... Tail>
    inline static void Write2(Head &&head, Tail&&... tail) {
    if (enabled && output == kClog && detail <= kError) {
        std::clog << head;
        Write2(std::forward<Tail>(tail)...);
    }
    }

    // When disabled nothing is written to log
#ifdef _DEBUG
    static constexpr bool enabled = true;
#else
    static constexpr bool enabled = false;
#endif
    // Setting below matter only when logging is enabled
    // Where to write log
    static constexpr Type output = kClog;
    // Show logs by severity
    // 0 - everything, 1 - info that deserves attention, 2 - important, like errors
    static constexpr Severity detail = kAll;
};
}; // util
