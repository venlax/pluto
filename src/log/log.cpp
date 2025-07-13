//
// Created by xixi on 2025-07-11.
//
#include "log/log.h"

namespace pluto {
    Logger::Logger(const LogType type, const std::string_view name) :type_(type), name_(name) {

    }


    static std::string wrap_str_without_color(const LogLevel level , std::string_view str) {
        auto level_str = level_to_str(level);
        return std::format("{:<7}: {}", level_str,str);
    }

    static std::string wrap_str_with_color(const LogLevel level , std::string_view str) {
        auto color = level_color(level);
        return std::format("{}{}{}",  color, wrap_str_without_color(level, str), "\033[0m");
    }


    static std::string wrap_str(LogLevel level, std::string_view str, bool console) {

        if (console) {
            return wrap_str_with_color(level, str);
        }
        return wrap_str_without_color(level, str);
    }



    void Logger::log(const LogLevel level,  std::string_view str) {
        if (log_level > level)
            return;
        // for (auto i = 0; i < 8; ++i) {
        //     // 注意不是2是1
        //     //if (2 << i & static_cast<uint8_t>(type_)) {
        //     if (1 << i & static_cast<uint8_t>(type_)) {
        //         for (auto& t_sinks = sinks_[static_cast<LogType>(1 << i)]; auto& sink : t_sinks) {
        //             sink->output(str);
        //         }
        //     }
        // }
        // 上述代码优化为如下
        const auto raw = static_cast<uint8_t>(type_);

        for (uint8_t bit = 1; bit != 0; bit <<= 1) {
            if (raw & bit) {
                for (auto& sink : sinks_[static_cast<LogType>(bit)]) {
                        sink->output(wrap_str(level, str, bit == static_cast<uint8_t>(LogType::CONSOLE)));
                }
            }
        }
    }

    std::expected<void, Error> Logger::addSink(const std::shared_ptr<LogSink>& sink, LogType type) {

        LogType real_type = type & type_;


        const auto raw = static_cast<uint8_t>(real_type);

        if (raw == 0) {
            return Error::error(1, "Logger's log_type doesn't match sink's log_type.");
        }


        // 第一步：检查是否有重复
        for (uint8_t bit = 1; bit != 0; bit <<= 1) {
            if (raw & bit) {
                if (auto key = static_cast<LogType>(bit); sinks_[key].contains(sink)) {
                    return Error::error(1, "Sink with log_type already exists. Note: no type inserted this sink.");
                }
            }
        }

        // 第二步：插入
        for (uint8_t bit = 1; bit != 0; bit <<= 1) {
            if (raw & bit) {
                auto key = static_cast<LogType>(bit);
                sinks_[key].insert(sink);
            }
        }

        return {};

        // for (auto i = 0; i < 8; ++i) {
        //     if (2 << i & static_cast<uint8_t>(real_type)) {
        //         auto& sinks = sinks_[static_cast<LogType>(2 << i)];
        //         if (sinks.contains(sink)) {
        //             return Error::error(1, "Sink with log_type already exists.");
        //         }
        //     }
        // }
        //
        // for (auto i = 0; i < 8; ++i) {
        //     if (2 << i & static_cast<uint8_t>(real_type)) {
        //         auto& sinks = sinks_[static_cast<LogType>(2 << i)];
        //         if (!sinks.contains(sink)) {
        //             sinks.insert(sink);
        //         }
        //     }
        // }
    }

}
