//
// Created by xixi on 2025-07-10.
//

#ifndef PLUTO_LOG_LEVEL_H
#define PLUTO_LOG_LEVEL_H

#include <array>
#include <cstdint>
#include <string_view>


namespace pluto {
    enum class LogLevel : uint8_t {
        INFO = 0,
        DEBUG = 1,
        WARNING = 2,
        ERROR = 3,
        FATAL = 4
    };


    constexpr std::array<std::string_view, 5> level_strs = {
        "INFO",
        "DEBUG",
        "WARNING",
        "ERROR",
        "FATAL"
    };

    constexpr std::array<std::string_view, 5> level_colors = {
        "\033[1;32m",
        "\033[1;34m",
        "\033[1;33m",
        "\033[1;31m",
        "\033[1;37;41m"
    };

    constexpr std::string_view level_to_str(LogLevel level) {
        return level_strs.at(static_cast<uint8_t>(level));
    }

    constexpr std::string_view level_color(LogLevel level) {
        return level_colors.at(static_cast<uint8_t>(level));
    }


}




#endif //PLUTO_LOG_LEVEL_H
