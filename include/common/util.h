//
// Created by xixi on 2025-07-13.
//

#ifndef PLUTO_INCLUDE_COMMON_UTIL_H
#define PLUTO_INCLUDE_COMMON_UTIL_H

#include <algorithm>
#include <string_view>
#include <ranges>

namespace pluto {
    inline bool isNumeric(std::string_view str) {
        return std::ranges::all_of(str, [](const unsigned char ch) {
            return std::isdigit(ch);
        });
    }
}


#endif //PLUTO_INCLUDE_COMMON_UTIL_H
