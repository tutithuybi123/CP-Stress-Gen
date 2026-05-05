#include "cp_stress_gen.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

static void assert_permutation(std::vector<int> values, const int first) {
    const int n = static_cast<int>(values.size());
    std::sort(values.begin(), values.end());
    for (int i = 0; i < n; ++i) {
        assert(values[static_cast<std::size_t>(i)] == first + i);
    }
}

int main() {
    cp_stress_gen::core::Random rng(5);

    assert(cp_stress_gen::Permutation(0).build(rng).empty());
    assert((cp_stress_gen::Permutation(1).zero_based().build(rng) == std::vector<int>{0}));

    const auto shuffled = cp_stress_gen::Permutation(20).shuffle().build(rng);
    assert_permutation(shuffled, 1);

    const auto zero_based = cp_stress_gen::Permutation(10).zero_based().shuffle().build(rng);
    assert_permutation(zero_based, 0);

    const auto reversed = cp_stress_gen::Permutation(5).reversed().build(rng);
    assert((reversed == std::vector<int>{5, 4, 3, 2, 1}));

    const auto almost = cp_stress_gen::Permutation(8).almost_sorted(3).build(rng);
    assert_permutation(almost, 1);

    const auto shifted = cp_stress_gen::Permutation(5).cyclic_shift(2).build(rng);
    assert((shifted == std::vector<int>{3, 4, 5, 1, 2}));

    const auto few = cp_stress_gen::Permutation(8).few_swaps(2).build(rng);
    assert_permutation(few, 1);

    const auto blocks = cp_stress_gen::Permutation(7).blocks(3).build(rng);
    assert((blocks == std::vector<int>{3, 2, 1, 6, 5, 4, 7}));

    cp_stress_gen::core::Random same_a(55);
    cp_stress_gen::core::Random same_b(55);
    assert(cp_stress_gen::Permutation(20).few_swaps(6).build(same_a) == cp_stress_gen::Permutation(20).few_swaps(6).build(same_b));

    bool thrown = false;
    try {
        (void)cp_stress_gen::Permutation(3).almost_sorted(4).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Permutation(3).few_swaps(4).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Permutation(3).blocks(0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "[PASS] test_permutation\n";
    return 0;
}
