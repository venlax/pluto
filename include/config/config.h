//
// Created by xixi on 2025-07-12.
//

#ifndef PLUTO_CONFIG_CONFIG_H
#define PLUTO_CONFIG_CONFIG_H

#include <any>
#include <expected>
#include <print>
#include <ranges>
#include <ranges>
#include <string>
#include <unordered_map>

#include "common/error.h"
#include "common/util.h"

#include <yaml-cpp/yaml.h>


namespace pluto {

    class Config {
    public:
        static Config& instance() {
            static Config instance;
            return instance;
        }

        std::optional<const std::any> operator[](std::string_view key) const;

    private:
        Config() {
            #ifndef CONFIG_PATH
            static_assert(false, "Macro CONFIG_PATH undefined.")
            #endif

            std::println("🔄 Config {} loading...", CONFIG_PATH);

            if (auto rt = init(CONFIG_PATH); !rt) {
                std::println(stderr, "Config initialization failed. errc: {} msg: {}", rt.error().code(), rt.error().msg());
                exit(-1);
            }

            std::println("✅ Config {} loaded", CONFIG_PATH);
        }

        std::unordered_map<std::string, std::any> configs_;

        std::expected<void, Error> init(std::string_view path);

        static std::expected<std::unordered_map<std::string, std::any>, Error> parse(const YAML::Node& root);

        static std::expected<void, Error> parse(const YAML::Node& node, std::any& value);
    };

    // 下面方案弃用
    // 保证在程序进入main之前初始化
    // 避免在调用时初始化
    // Config Config::s_instance;

}


#endif //PLUTO_CONFIG_CONFIG_H
