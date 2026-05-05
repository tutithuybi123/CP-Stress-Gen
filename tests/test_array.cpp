#include "cp_stress_gen.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    cp_stress_gen::core::Random rng(1);

    assert(cp_stress_gen::Array(0).range(1, 10).build(rng).empty());
    assert((cp_stress_gen::Array(1).iota(7).build(rng) == std::vector<long long>{7}));

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

    auto almost = cp_stress_gen::Array(8).almost_sorted(3).build(rng);
    std::sort(almost.begin(), almost.end());
    assert((almost == std::vector<long long>{1, 2, 3, 4, 5, 6, 7, 8}));

    const auto equal = cp_stress_gen::Array(30).many_equal(4).build(rng);
    assert(equal.size() == 30);
    for (const auto value : equal) {
        assert(value >= 1 && value <= 4);
    }

    const auto blocky = cp_stress_gen::Array(7).blocky(3).build(rng);
    assert((blocky == std::vector<long long>{1, 1, 1, 2, 2, 2, 3}));

    cp_stress_gen::core::Random same_a(77);
    cp_stress_gen::core::Random same_b(77);
    assert(cp_stress_gen::Array(20).almost_sorted(5).build(same_a) == cp_stress_gen::Array(20).almost_sorted(5).build(same_b));

    bool thrown = false;
    try {
        (void)cp_stress_gen::Array(3).range(5, 1);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Array(3).many_equal(0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Array(3).almost_sorted(4);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Array(3).blocky(0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "[PASS] test_array\n";
    return 0;
}
