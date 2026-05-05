#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

namespace cp_stress_gen {

class Array {
public:
    using value_type = long long;
    using size_type = std::size_t;

    explicit Array(const size_type size) : size_(size) {}

    Array& range(const value_type left, const value_type right) {
        core::require_range(left, right, "Array::range requires left <= right");
        mode_ = Mode::Range;
        left_ = left;
        right_ = right;
        return *this;
    }

    Array& fill(const value_type value) noexcept {
        mode_ = Mode::Fill;
        fill_value_ = value;
        return *this;
    }

    Array& iota(const value_type start = 0, const value_type step = 1) noexcept {
        mode_ = Mode::Iota;
        start_ = start;
        step_ = step;
        return *this;
    }

    Array& shuffle() noexcept {
        shuffle_ = true;
        return *this;
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] std::vector<value_type> build(core::Random& rng) const {
        std::vector<value_type> values = build_values(rng);
        if (shuffle_) {
            shuffle_values(values, rng);
        }
        return values;
    }

    [[nodiscard]] std::vector<value_type> build() const {
        core::Random rng = core::Random::from_time();
        return build(rng);
    }

private:
    enum class Mode {
        Fill,
        Iota,
        Range
    };

    size_type size_;
    Mode mode_{Mode::Fill};
    value_type fill_value_{0};
    value_type start_{0};
    value_type step_{1};
    value_type left_{0};
    value_type right_{0};
    bool shuffle_{false};

    [[nodiscard]] std::vector<value_type> build_values(core::Random& rng) const {
        std::vector<value_type> values(size_);
        if (mode_ == Mode::Fill) {
            std::fill(values.begin(), values.end(), fill_value_);
        } else if (mode_ == Mode::Iota) {
            value_type current = start_;
            for (size_type i = 0; i < size_; ++i) {
                values[i] = current;
                current += step_;
            }
        } else {
            for (size_type i = 0; i < size_; ++i) {
                values[i] = rng.integer<value_type>(left_, right_);
            }
        }
        return values;
    }

    static void shuffle_values(std::vector<value_type>& values, core::Random& rng) {
        if (values.size() < 2) {
            return;
        }

        for (size_type i = values.size() - 1; i > 0; --i) {
            const size_type j = rng.integer<size_type>(0, i);
            std::swap(values[i], values[j]);
        }
    }
};

} // namespace cp_stress_gen

