#pragma once

#include <stdexcept>
#include <string>

namespace cp_stress_gen::core {

inline void require(const bool condition, const std::string& message) {
    if (!condition) {
        throw std::invalid_argument(message);
    }
}

template <typename T>
inline void require_range(const T& left, const T& right, const std::string& message) {
    require(!(right < left), message);
}

template <typename T>
inline void require_positive(const T& value, const std::string& message) {
    require(value > T{}, message);
}

} // namespace cp_stress_gen::core

