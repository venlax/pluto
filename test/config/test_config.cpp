//
// Created by xixi on 2025-07-13.
//
#include "config/config.h"


#define TEST(key, type) \
{ \
const auto val = config[key]; \
if (val.has_value()) { \
std::println("{} = {}", key, std::any_cast<type>(val.value())); \
} else { \
std::println("{}: Not found", key); \
} \
}


int main() {
    auto config = pluto::Config::instance();

    // server.*
    TEST("server.host", std::string);
    TEST("server.port", std::string);
    TEST("server.enable_https", std::string);

    // database.*
    TEST("database.type", std::string);
    TEST("database.connection.host", std::string);
    TEST("database.connection.port", std::string);
    TEST("database.connection.user", std::string);
    TEST("database.connection.password", std::string);

    // features.*
    TEST("features.logging.level", std::string);
    TEST("features.logging.sinks.0", std::string);
    TEST("features.logging.sinks.1", std::string);
    TEST("features.cache.enabled", std::string);
    TEST("features.cache.max_items", std::string);
    TEST("features.cache.strategy", std::string);

    // users.*
    TEST("users.0.name", std::string);
    TEST("users.0.id", std::string);
    TEST("users.0.roles.0", std::string);
    TEST("users.0.roles.1", std::string);
    TEST("users.1.name", std::string);
    TEST("users.1.id", std::string);
    TEST("users.1.roles.0", std::string);

    // thresholds.*
    TEST("thresholds.cpu", std::string);
    TEST("thresholds.memory", std::string);
    TEST("thresholds.disk", std::string);

}




// useless code
// auto config = pluto::Config::instance();
//
// const auto rt = config["logger.name"];
//
// if (rt.has_value()) {
//     auto res = std::any_cast<std::string>(rt.value());
//     std::println("{}", res);
// } else {
//     std::println("No!");
// }
//
// return 0;