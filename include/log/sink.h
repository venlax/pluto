//
// Created by xixi on 2025-07-11.
//

#ifndef PLUTO_LOG_SINK_H
#define PLUTO_LOG_SINK_H

#include <concepts>
#include <cstdio>
#include <memory>
#include <ostream>
#include <print>

#include "type.h"

namespace pluto {


    class LogSink {
    public:

        using ptr = std::shared_ptr<LogSink>;

        LogSink() = default;

        virtual ~LogSink() = default;

        virtual void output(std::string_view str) = 0;
    };


    template<typename T>
    concept Outputable =
        std::same_as<T, std::FILE*> ||
        std::derived_from<T, std::basic_ostream<char>>;

    template<typename T>
    requires Outputable<T>
    class TLogSink final : public LogSink{
    public:

        explicit TLogSink(T& sink);

        ~TLogSink() override;

        void output(std::string_view str) override;
    private:

        T& sink_;
    };

}

#include "impl/sink.impl.h"

#endif //PLUTO_LOG_SINK_H
