#include "cp_stress_gen.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    assert(cp_stress_gen::Math::gcd(48, 18) == 6);
    assert(cp_stress_gen::Math::gcd(-48, 18) == 6);
    assert(cp_stress_gen::Math::lcm(12, 18) == 36);
    assert(cp_stress_gen::Math::lcm(0, 18) == 0);

    assert((cp_stress_gen::Math::divisors(12) == std::vector<long long>{1, 2, 3, 4, 6, 12}));
    assert((cp_stress_gen::Math::primes_up_to(10) == std::vector<int>{2, 3, 5, 7}));
    assert(cp_stress_gen::Math::is_prime(97));
    assert(cp_stress_gen::Math::is_prime(2));
    assert(!cp_stress_gen::Math::is_prime(1));
    assert(!cp_stress_gen::Math::is_prime(100));
    assert(!cp_stress_gen::Math::is_prime(-7));

    cp_stress_gen::core::Random rng(6);
    for (int i = 0; i < 100; ++i) {
        const int prime = cp_stress_gen::Math::random_prime(10, 30, rng);
        assert(prime == 11 || prime == 13 || prime == 17 || prime == 19 || prime == 23 || prime == 29);
    }

    for (int i = 0; i < 100; ++i) {
        const long long composite = cp_stress_gen::Math::random_composite(10, 20, rng);
        assert(composite >= 10 && composite <= 20);
        assert(!cp_stress_gen::Math::is_prime(composite));
    }

    const auto coprime = cp_stress_gen::Math::coprime_pair(10, 20, rng);
    assert(cp_stress_gen::Math::gcd(coprime.first, coprime.second) == 1);

    const auto with_gcd = cp_stress_gen::Math::with_gcd(6, 2, 9, rng);
    assert(cp_stress_gen::Math::gcd(with_gcd.first, with_gcd.second) == 6);

    assert(cp_stress_gen::Math::mod_pow(2, 10, 1000) == 24);
    assert(cp_stress_gen::Math::mod_pow(-2, 3, 5) == 2);

    const auto eg = cp_stress_gen::Math::extended_gcd(30, 18);
    assert(eg.gcd == 6);
    assert(30 * eg.x + 18 * eg.y == eg.gcd);

    assert(cp_stress_gen::Math::mod_inverse(3, 11) == 4);
    assert((cp_stress_gen::Math::factorize_trial(360) == std::vector<std::pair<long long, int>>{{2, 3}, {3, 2}, {5, 1}}));
    assert(cp_stress_gen::Math::factorize_trial(1).empty());

    cp_stress_gen::core::Random same_a(11);
    cp_stress_gen::core::Random same_b(11);
    assert(cp_stress_gen::Math::random_composite(4, 30, same_a) == cp_stress_gen::Math::random_composite(4, 30, same_b));

    bool thrown = false;
    try {
        (void)cp_stress_gen::Math::mod_pow(2, -1, 5);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::mod_inverse(2, 4);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::random_prime(14, 16, rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::random_composite(2, 3, rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::random_composite(10, 2, rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::coprime_pair(2, 2, rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::with_gcd(0, 1, 5, rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::with_gcd(3, 0, 5, rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "[PASS] test_math\n";
    return 0;
}
