#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <cstddef>
#include <utility>
#include <vector>

namespace cp_stress_gen::anti {

class SortKiller {
public:
    using value_type = long long;
    using size_type = std::size_t;

    [[nodiscard]] static std::vector<value_type> reversed(const size_type n, const value_type first = 1) {
        std::vector<value_type> result;
        result.reserve(n);
        for (size_type i = 0; i < n; ++i) {
            result.push_back(first + static_cast<value_type>(n - 1 - i));
        }
        return result;
    }

    [[nodiscard]] static std::vector<value_type> nearly_sorted(const size_type n, const size_type swaps, core::Random& rng) {
        core::require(swaps <= n, "SortKiller::nearly_sorted swaps must be <= n");
        std::vector<value_type> result;
        result.reserve(n);
        for (size_type i = 0; i < n; ++i) {
            result.push_back(static_cast<value_type>(i + 1));
        }
        for (size_type i = 0; i < swaps && n > 1; ++i) {
            const size_type a = rng.integer<size_type>(0, n - 1);
            const size_type b = rng.integer<size_type>(0, n - 1);
            std::swap(result[a], result[b]);
        }
        return result;
    }

    [[nodiscard]] static std::vector<value_type> many_duplicates(const size_type n, const size_type distinct, core::Random& rng) {
        core::require_positive(distinct, "SortKiller::many_duplicates distinct count must be positive");
        std::vector<value_type> result;
        result.reserve(n);
        for (size_type i = 0; i < n; ++i) {
            result.push_back(static_cast<value_type>(rng.integer<size_type>(1, distinct)));
        }
        return result;
    }
};

} // namespace cp_stress_gen::anti

