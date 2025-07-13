//
// Created by xixi on 2025-07-11.
//

#ifndef PLUTO_LOG_TYPE_H
#define PLUTO_LOG_TYPE_H
#include <cstdint>

namespace pluto {
    enum class LogType : uint8_t {
        CONSOLE = 1 << 0,
        FILE = 1 << 1
    };

    inline LogType operator|(LogType lhs, LogType rhs) {
        return static_cast<LogType>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
    }

    inline LogType operator&(LogType lhs, LogType rhs) {
        return static_cast<LogType>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
    }
}

#endif //PLUTO_LOG_TYPE_H
