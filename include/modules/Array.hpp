#pragma once

#include "../core/Random.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <utility>
#include <vector>

namespace cp_stress_gen {

class Array {
public:
    using value_type = long long;
    using size_type = std::size_t;

    explicit Array(const size_type size)
        : values_(size), random_(core::Random::from_time()) {}

    Array(const size_type size, const std::uint64_t seed)
        : values_(size), random_(seed) {}

    Array(const size_type size, core::Random random) noexcept
        : values_(size), random_(random) {}

    [[nodiscard]] static Array seeded(const size_type size, const std::uint64_t seed) {
        return Array(size, seed);
    }

    Array& seed(const std::uint64_t seed_value) noexcept {
        random_.seed(seed_value);
        return *this;
    }

    Array& range(value_type left, value_type right) noexcept {
        if (right < left) {
            std::swap(left, right);
        }

        for (const size_type i : std::views::iota(size_type{0}, values_.size())) {
            values_[i] = random_.integer<value_type>(left, right);
        }
        return *this;
    }

    Array& fill(const value_type value) noexcept {
        std::ranges::fill(values_, value);
        return *this;
    }

    Array& iota(value_type start = 0, const value_type step = 1) noexcept {
        for (const size_type i : std::views::iota(size_type{0}, values_.size())) {
            values_[i] = start;
            start += step;
        }
        return *this;
    }

    Array& shuffle() noexcept {
        if (values_.size() < 2) {
            return *this;
        }

        for (size_type i = values_.size() - 1; i > 0; --i) {
            const size_type j = random_.integer<size_type>(0, i);
            std::swap(values_[i], values_[j]);
        }
        return *this;
    }

    [[nodiscard]] size_type size() const noexcept {
        return values_.size();
    }

    [[nodiscard]] const std::vector<value_type>& view() const noexcept {
        return values_;
    }

    [[nodiscard]] std::vector<value_type> build() const& {
        return values_;
    }

    [[nodiscard]] std::vector<value_type> build() && noexcept {
        return std::move(values_);
    }

private:
    std::vector<value_type> values_;
    core::Random random_;
};

} // namespace cp_stress_gen

