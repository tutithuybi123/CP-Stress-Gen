#include "cp_stress_gen.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <vector>

int main() {
    cp_stress_gen::core::Random rng(1);

    const auto values = cp_stress_gen::Array(100).range(-10, 10).build(rng);
    assert(values.size() == 100);
    for (const auto value : values) {
        assert(value >= -10 && value <= 10);
    }

    const auto filled = cp_stress_gen::Array(5).fill(42).build(rng);
    assert((filled == std::vector<long long>{42, 42, 42, 42, 42}));

    const auto sequence = cp_stress_gen::Array(5).iota(3, 2).build(rng);
    assert((sequence == std::vector<long long>{3, 5, 7, 9, 11}));

    auto shuffled = cp_stress_gen::Array(5).iota(1).shuffle().build(rng);
    std::sort(shuffled.begin(), shuffled.end());
    assert((shuffled == std::vector<long long>{1, 2, 3, 4, 5}));

    bool thrown = false;
    try {
        (void)cp_stress_gen::Array(3).range(5, 1);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    return 0;
}

