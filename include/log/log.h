//
// Created by xixi on 2025-07-10.
//

#ifndef PLUTO_LOG_H
#define PLUTO_LOG_H


#include <expected>
#include <memory>
#include <format>
#include <unordered_map>
#include <unordered_set>

#include "level.h"
#include "sink.h"
#include "type.h"
#include "common/error.h"


namespace pluto {

    static LogLevel log_level = LogLevel::INFO;

    class Logger {

    public:

        using ptr = std::shared_ptr<Logger>;

        explicit Logger(LogType type, std::string_view name);

        ~Logger() = default;

        void info(std::string_view str) {
            log(LogLevel::INFO, str);
        }

        void debug(std::string_view str) {
            log(LogLevel::DEBUG, str);
        }

        void warn(std::string_view str) {
            log(LogLevel::WARNING, str);
        }

        void error(std::string_view str) {
            log(LogLevel::ERROR, str);
        }

        void fatal(std::string_view str) {
            log(LogLevel::FATAL, str);
        }

        template <typename... Args>
        void info(std::format_string<Args...> fmt, Args&&... args) {
            log(LogLevel::INFO, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void debug(std::format_string<Args...> fmt, Args&&... args) {
            log(LogLevel::DEBUG, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void warn(std::format_string<Args...> fmt, Args&&... args) {
            log(LogLevel::WARNING, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void error(std::format_string<Args...> fmt, Args&&... args) {
            log(LogLevel::ERROR, fmt, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void fatal(std::format_string<Args...> fmt, Args&&... args) {
            log(LogLevel::FATAL, fmt, std::forward<Args>(args)...);
        }



        std::string_view name() const { return name_; }

        std::expected<void, Error> addSink(const std::shared_ptr<LogSink>& sink, LogType type);

    private:

        LogType type_;

        std::string name_;

        std::unordered_map<LogType, std::unordered_set<LogSink::ptr>> sinks_;

        void log(LogLevel level,  std::string_view str);

        template <typename... Args>
        void log(const LogLevel level, std::format_string<Args...> fmt, Args&&... args) {
            const std::string str = std::format(fmt, std::forward<Args>(args)...);
            log(level, str);
        }

    };


}


#endif //PLUTO_LOG_H
