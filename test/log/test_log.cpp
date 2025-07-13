//
// Created by xixi on 2025-07-11.
//

#include "log/log.h"

int main() {
    pluto::Logger::ptr logger = std::make_shared<pluto::Logger>(
        pluto::LogType::CONSOLE | pluto::LogType::FILE,
        "root"
        );
    auto ptr = std::make_shared<pluto::TLogSink<FILE *> >(stdout);
    if (auto rt = logger->addSink(ptr,
                                  pluto::LogType::CONSOLE)) {
        logger->info("Hello World, Pluto!");
        logger->debug("This is pluto Web Server, and 1 + 1 = {}", 1 + 1);
    } else {
        std::println(stderr, "errc: {}, msg: {}", rt.error().code(), rt.error().msg());
    }

    if (auto rt = logger->addSink(ptr,
                              pluto::LogType::CONSOLE)) {
        logger->info("Hello World, Pluto!");
        logger->warn("This is pluto Web Server, and 1 + 1 = {}", 1 + 1);
    } else {
          std::println(stderr, "errc: {}, msg: {}", rt.error().code(), rt.error().msg());
    }

    // should not do this
    // pluto::LogSink* sink_raw = new pluto::TLogSink(stderr);
    //
    // std::shared_ptr<pluto::LogSink> ptr1(sink_raw);
    // decltype(ptr1) ptr2(sink_raw);
    //
    // logger->addSink(ptr1, pluto::LogType::CONSOLE);
    //
    // if (auto rt = logger->addSink(ptr2,
    //                           pluto::LogType::CONSOLE)) {
    //     logger->log("Hello World, Pluto!");
    //     logger->log("This is pluto Web Server, and 1 + 1 = {}", 1 + 1);
    // } else {
    //   std::println(stderr, "errc: {}, msg: {}", rt.error().code(), rt.error().msg());
    // }


    pluto::Logger::ptr logger2 = std::make_shared<pluto::Logger>(
    pluto::LogType::CONSOLE ,
    "root2"
    );

    FILE* fp = fopen("log.txt", "w");

    if (auto rt = logger2->addSink(std::make_shared<pluto::TLogSink<FILE *> >(fp),
                                  pluto::LogType::FILE)) {
        logger2->error("Hello World, Pluto!");
        logger2->fatal("This is pluto Web Server, and 1 + 1 = {}", 1 + 1);
    } else {
          std::println(stderr, "errc: {}, msg: {}", rt.error().code(), rt.error().msg());
    }

    return 0;
}