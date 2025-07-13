//
// Created by xixi on 2025-07-13.
//
#include "config/config.h"



namespace pluto {
    std::optional<const std::any> Config::operator[](std::string_view key) const {
        auto subranges = std::views::split(key, '.');

        std::any cur_node = std::ref(configs_).get();

        for (auto&& token : subranges) {
            std::string part(token.begin(), std::ranges::distance(token));

            if (isNumeric(part)) {
                // may be the vector access
                if (const auto seq = std::any_cast<std::vector<std::any>>(&cur_node); seq != nullptr) {
                    cur_node = (*seq)[std::stoi(part)];
                    continue;
                }
            }
            const auto map = std::any_cast<std::unordered_map<std::string, std::any>>(&cur_node);


            if (map == nullptr || !map->contains(part)) {
                return std::nullopt;
            }

            cur_node = (*map)[part];
        }

        return cur_node;

    }


    // static
    std::expected<void, Error> Config::init(std::string_view path) {

        YAML::Node root;
        try {
            root = YAML::LoadFile(path.data());
        } catch (const YAML::Exception& e) {
            return Error::error(1, "YAML::LoadFile failed, error: {}", e.msg);
        }

        if (auto rt = parse(root); rt) {
            this->configs_ = std::move(rt.value());
        } else {
            return Error::error(rt.error().code(), "Parse failed, error: {}", rt.error().msg());
        }

        return {};
    }

    // static
    std::expected<std::unordered_map<std::string, std::any>, Error> Config::parse(const YAML::Node& root) {

        if (!root.IsMap()) {
            return Error::error(1,"Config yaml file root must be a map aka. key-value.");
        }
        std::any configs_wrapper;
        if (auto rt = parse(root, configs_wrapper); rt) {
            return std::any_cast<std::unordered_map<std::string, std::any>>(configs_wrapper);
        } else {
            return Error::error(1,"Parse failed, error: {}", rt.error().msg());
        }
    }

    std::expected<void, Error> Config::parse(const YAML::Node& node, std::any& value) {

        if (node.IsScalar()) {
            try {
                value = node.as<std::string>();
            } catch (const YAML::Exception& e) {
                return Error::error(1,"Parse failed, error: {}", e.msg);
            }
        } else if (node.IsSequence()) {
            if (!value.has_value()) {
                value = std::vector<std::any>();
            }
            const auto seq = std::any_cast<std::vector<std::any>>(&value);

            if (seq == nullptr) {
                return Error::error(1, "Parse failed, error: can't cast std::any to std::vector.");
            }

            for (auto& iter : node) {
                std::any new_value;
                if (auto rt = parse(iter, new_value); rt) {
                    seq->emplace_back(new_value);
                } else {
                    return Error::error(1,"Parse failed, error: {}", rt.error().msg());
                }
            }
        } else if (node.IsMap()) {
            if (!value.has_value()) {
                value = std::unordered_map<std::string, std::any>();
            }
            const auto map = std::any_cast<std::unordered_map<std::string, std::any>>(&value);

            if (map == nullptr) {
                return Error::error(1, "Parse failed, error: can't cast std::any to std::map.");
            }
            for (auto& iter : node) {
                std::any new_value;
                std::string key;
                try {
                    key = iter.first.as<std::string>();
                } catch (const YAML::Exception& e) {
                    return Error::error(1,"Parse failed, error: {}", e.msg);
                }
                if (auto rt = parse(iter.second, new_value); rt) {
                    map->emplace(std::move(key), std::move(new_value));
                } else {
                    return Error::error(1,"Parse failed, error: {}", rt.error().msg());
                }
            }

        } else if (node.IsNull()) {
            return Error::error(1, "Parse failed, error: null node is not allowed in config file.");
        }

        return {};
    }


}