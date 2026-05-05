#pragma once

#include "../core/Validate.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace cp_stress_gen::anti {

class StringKiller {
public:
    using size_type = std::size_t;

    [[nodiscard]] static std::string repeated(const size_type n, const char ch = 'a') {
        return std::string(n, ch);
    }

    [[nodiscard]] static std::string alternating(const size_type n, const char a = 'a', const char b = 'b') {
        std::string result;
        result.reserve(n);
        for (size_type i = 0; i < n; ++i) {
            result.push_back(i % 2 == 0 ? a : b);
        }
        return result;
    }

    [[nodiscard]] static std::vector<std::string> prefix_heavy(const size_type count, const size_type length) {
        core::require(length > 0 || count == 0, "StringKiller::prefix_heavy length must be positive when count > 0");
        std::vector<std::string> result;
        result.reserve(count);
        for (size_type i = 0; i < count; ++i) {
            std::string value(length, 'a');
            value[length - 1] = static_cast<char>('a' + static_cast<int>(i % 26));
            result.push_back(value);
        }
        return result;
    }

    [[nodiscard]] static std::string kmp_prefix_pattern(const size_type n, const char repeated = 'a', const char breaker = 'b') {
        if (n == 0) {
            return {};
        }
        std::string result(n, repeated);
        if (n > 1) {
            result[n - 1] = breaker;
        }
        return result;
    }
};

} // namespace cp_stress_gen::anti
