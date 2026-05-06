#pragma once

#include "Validate.hpp"

#include <initializer_list>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>

namespace cp_stress_gen::core {

class Options {
public:
    Options(const int argc, char** argv) {
        for (int index = 1; index < argc; ++index) {
            const std::string argument(argv[index]);
            if (argument == "-h" || argument == "--help") {
                add_option("help", "true", false);
                continue;
            }

            require(starts_with(argument, "--"), "Options supports only --name, --name=value, and -h");
            std::string token = argument.substr(2);
            require(!token.empty(), "Options option name cannot be empty");

            const std::string::size_type equals = token.find('=');
            if (equals != std::string::npos) {
                const std::string name = token.substr(0, equals);
                const std::string value = token.substr(equals + 1);
                require(!name.empty(), "Options option name cannot be empty");
                require(!value.empty(), "Options option value cannot be empty");
                add_option(name, value, true);
                continue;
            }

            if (index + 1 < argc) {
                const std::string next(argv[index + 1]);
                if (is_value_token(next)) {
                    add_option(token, next, true);
                    ++index;
                    continue;
                }
            }

            add_option(token, "true", false);
        }
    }

    [[nodiscard]] bool has(const std::string& name) const {
        return options_.find(normalize_name(name)) != options_.end();
    }

    [[nodiscard]] bool flag(const std::string& name, const bool default_value = false) const {
        const auto it = options_.find(normalize_name(name));
        if (it == options_.end()) {
            return default_value;
        }
        if (!it->second.has_explicit_value) {
            return true;
        }
        return parse_bool(it->second.value, it->first);
    }

    template <typename T>
    [[nodiscard]] T get(const std::string& name, const T& default_value) const {
        const auto it = options_.find(normalize_name(name));
        if (it == options_.end()) {
            return default_value;
        }
        return read_value<T>(it->first, it->second);
    }

    template <typename T>
    [[nodiscard]] T required(const std::string& name) const {
        const std::string normalized = normalize_name(name);
        const auto it = options_.find(normalized);
        require(it != options_.end(), "Options missing required option: " + normalized);
        return read_value<T>(it->first, it->second);
    }

    void require_no_unknown(const std::initializer_list<std::string> known_names) const {
        std::set<std::string> known;
        for (const auto& name : known_names) {
            known.insert(normalize_name(name));
        }
        for (const auto& option : options_) {
            require(known.find(option.first) != known.end(), "Options unknown option: " + option.first);
        }
    }

private:
    struct Entry {
        std::string value;
        bool has_explicit_value;
    };

    std::map<std::string, Entry> options_;

    static bool starts_with(const std::string& value, const std::string& prefix) {
        return value.compare(0, prefix.size(), prefix) == 0;
    }

    static bool is_value_token(const std::string& value) {
        return value != "-h" && !starts_with(value, "--");
    }

    static std::string normalize_name(const std::string& name) {
        if (name == "-h") {
            return "help";
        }
        if (starts_with(name, "--")) {
            return name.substr(2);
        }
        return name;
    }

    void add_option(const std::string& name, const std::string& value, const bool has_explicit_value) {
        const std::string normalized = normalize_name(name);
        require(!normalized.empty(), "Options option name cannot be empty");
        require(options_.find(normalized) == options_.end(), "Options duplicate option: " + normalized);
        options_[normalized] = Entry{value, has_explicit_value};
    }

    template <typename T>
    [[nodiscard]] typename std::enable_if<std::is_same<T, std::string>::value, T>::type read_value(
        const std::string& name,
        const Entry& entry
    ) const {
        require(entry.has_explicit_value, "Options option requires a value: " + name);
        return entry.value;
    }

    template <typename T>
    [[nodiscard]] typename std::enable_if<std::is_same<T, bool>::value, T>::type read_value(
        const std::string& name,
        const Entry& entry
    ) const {
        if (!entry.has_explicit_value) {
            return true;
        }
        return parse_bool(entry.value, name);
    }

    template <typename T>
    [[nodiscard]] typename std::enable_if<
        std::is_integral<T>::value && !std::is_same<T, bool>::value,
        T
    >::type read_value(const std::string& name, const Entry& entry) const {
        require(entry.has_explicit_value, "Options option requires a value: " + name);
        long long parsed = 0;
        std::istringstream input(entry.value);
        input >> parsed;
        require(!input.fail() && input.eof(), "Options invalid integer value for: " + name);
        const long long min_value = std::is_unsigned<T>::value
            ? 0LL
            : static_cast<long long>(std::numeric_limits<T>::min());
        const long long max_value = static_cast<long long>(std::numeric_limits<T>::max());
        require(parsed >= min_value && parsed <= max_value, "Options integer value out of range for: " + name);
        return static_cast<T>(parsed);
    }

    template <typename T>
    [[nodiscard]] typename std::enable_if<std::is_floating_point<T>::value, T>::type read_value(
        const std::string& name,
        const Entry& entry
    ) const {
        require(entry.has_explicit_value, "Options option requires a value: " + name);
        T parsed{};
        std::istringstream input(entry.value);
        input >> parsed;
        require(!input.fail() && input.eof(), "Options invalid floating-point value for: " + name);
        return parsed;
    }

    static bool parse_bool(const std::string& value, const std::string& name) {
        if (value == "1" || value == "true" || value == "yes" || value == "on") {
            return true;
        }
        if (value == "0" || value == "false" || value == "no" || value == "off") {
            return false;
        }
        throw std::invalid_argument("Options invalid boolean value for: " + name);
    }
};

} // namespace cp_stress_gen::core
