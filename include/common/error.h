//
// Created by xixi on 2025-07-12.
//

#ifndef PLUTO_COMMON_ERROR_H
#define PLUTO_COMMON_ERROR_H
#include <cstdint>
#include <string_view>

namespace pluto {

    // #include "errc_define.h"


    class Error {
    public:

        static std::unexpected<Error> error(const uint8_t code, std::string_view msg) {
            return std::unexpected(Error(code, msg));
        }

        template <typename... Args>
        static std::unexpected<Error> error(const uint8_t code, std::format_string<Args...> fmt, Args&&... args) {
            const std::string msg = std::format(fmt, std::forward<Args>(args)...);
            return error(code, msg);
        }

        explicit Error(const uint8_t code, std::string_view msg) : code_(code), msg_(msg) {};
        ~Error() = default;

        [[nodiscard]] uint8_t code() const { return code_; }
        [[nodiscard]] const std::string& msg() const { return msg_; }

    private:
        uint8_t code_;
        std::string msg_;
    };
}



#endif //PLUTO_COMMON_ERROR_H
