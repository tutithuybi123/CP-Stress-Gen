#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

namespace cp_stress_gen {

class Permutation {
public:
    using value_type = int;
    using size_type = std::size_t;

    explicit Permutation(const size_type size) : size_(size) {}

    Permutation& one_based() noexcept {
        first_ = 1;
        return *this;
    }

    Permutation& zero_based() noexcept {
        first_ = 0;
        return *this;
    }

    Permutation& shuffle() noexcept {
        mode_ = Mode::Shuffle;
        return *this;
    }

    Permutation& reversed() noexcept {
        mode_ = Mode::Reversed;
        return *this;
    }

    Permutation& almost_sorted(const size_type swaps) noexcept {
        mode_ = Mode::AlmostSorted;
        swaps_ = swaps;
        return *this;
    }

    [[nodiscard]] size_type size() const noexcept {
        return size_;
    }

    [[nodiscard]] std::vector<value_type> build(core::Random& rng) const {
        core::require(size_ <= static_cast<size_type>(2147483647), "Permutation size exceeds int value capacity");
        if (mode_ == Mode::AlmostSorted) {
            core::require(swaps_ <= size_, "Permutation::almost_sorted swaps must be <= n");
        }

        std::vector<value_type> result(size_);
        for (size_type i = 0; i < size_; ++i) {
            result[i] = static_cast<value_type>(first_ + static_cast<value_type>(i));
        }

        if (mode_ == Mode::Shuffle) {
            shuffle_values(result, rng);
        } else if (mode_ == Mode::Reversed) {
            std::reverse(result.begin(), result.end());
        } else if (mode_ == Mode::AlmostSorted) {
            for (size_type i = 0; i < swaps_; ++i) {
                if (result.size() < 2) {
                    break;
                }
                const size_type a = rng.integer<size_type>(0, result.size() - 1);
                const size_type b = rng.integer<size_type>(0, result.size() - 1);
                std::swap(result[a], result[b]);
            }
        }

        return result;
    }

    [[nodiscard]] std::vector<value_type> build() const {
        core::Random rng = core::Random::from_time();
        return build(rng);
    }

private:
    enum class Mode {
        Identity,
        Shuffle,
        Reversed,
        AlmostSorted
    };

    size_type size_;
    value_type first_{1};
    size_type swaps_{0};
    Mode mode_{Mode::Identity};

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

