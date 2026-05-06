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

    const auto spf = cp_stress_gen::Math::sieve_spf(20);
    assert(spf[1] == 1);
    assert(spf[18] == 2);
    assert(spf[19] == 19);
    assert((cp_stress_gen::Math::factorize_spf(18, spf) == std::vector<std::pair<long long, int>>{{2, 1}, {3, 2}}));

    assert(cp_stress_gen::Math::is_prime(97));
    assert(cp_stress_gen::Math::is_prime(2));
    assert(!cp_stress_gen::Math::is_prime(1));
    assert(!cp_stress_gen::Math::is_prime(100));
    assert(!cp_stress_gen::Math::is_prime(-7));
    assert(cp_stress_gen::Math::euler_phi(1) == 1);
    assert(cp_stress_gen::Math::euler_phi(9) == 6);
    assert(cp_stress_gen::Math::euler_phi(36) == 12);

    const auto mobius = cp_stress_gen::Math::mobius_up_to(10);
    assert(mobius[1] == 1);
    assert(mobius[2] == -1);
    assert(mobius[4] == 0);
    assert(mobius[6] == 1);

    const std::vector<std::pair<long long, int>> factors360{{2, 3}, {3, 2}, {5, 1}};
    assert(cp_stress_gen::Math::divisor_count_from_factorization(factors360) == 24);
    assert(cp_stress_gen::Math::divisor_sum_from_factorization(factors360) == 1170);

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
    assert(cp_stress_gen::Math::mod_add(8, 7, 10) == 5);
    assert(cp_stress_gen::Math::mod_mul(12, 13, 17) == 3);

    const auto eg = cp_stress_gen::Math::extended_gcd(30, 18);
    assert(eg.gcd == 6);
    assert(30 * eg.x + 18 * eg.y == eg.gcd);

    assert(cp_stress_gen::Math::mod_inverse(3, 11) == 4);
    const auto crt_ok = cp_stress_gen::Math::crt_pair(2, 3, 3, 5);
    assert(crt_ok.compatible);
    assert(crt_ok.value == 8);
    assert(crt_ok.mod == 15);

    const auto crt_bad = cp_stress_gen::Math::crt_pair(1, 2, 2, 4);
    assert(!crt_bad.compatible);
    assert(crt_bad.mod == 4);

    assert((cp_stress_gen::Math::factorize_trial(360) == std::vector<std::pair<long long, int>>{{2, 3}, {3, 2}, {5, 1}}));
    assert(cp_stress_gen::Math::factorize_trial(1).empty());
    assert(cp_stress_gen::Math::binom_small(5, 2) == 10);
    assert(cp_stress_gen::Math::binom_small(6, 3) == 20);
    assert(cp_stress_gen::Math::binom_small(5, 7) == 0);
    assert((cp_stress_gen::Math::factorials_mod(5, 100) == std::vector<long long>{1, 1, 2, 6, 24, 20}));

    cp_stress_gen::core::Random same_a(11);
    cp_stress_gen::core::Random same_b(11);
    assert(cp_stress_gen::Math::random_composite(4, 30, same_a) == cp_stress_gen::Math::random_composite(4, 30, same_b));

    for (int i = 0; i < 100; ++i) {
        const int prime = cp_stress_gen::Math::random_prime_sieve(10, 30, rng);
        assert(prime == 11 || prime == 13 || prime == 17 || prime == 19 || prime == 23 || prime == 29);
    }

    const auto random_coprime = cp_stress_gen::Math::random_coprime_pair(10, 20, rng);
    assert(cp_stress_gen::Math::gcd(random_coprime.first, random_coprime.second) == 1);

    const int divisor_rich = cp_stress_gen::Math::random_number_with_many_divisors(20, rng);
    assert(divisor_rich >= 1 && divisor_rich <= 20);
    assert(cp_stress_gen::Math::divisors(divisor_rich).size() == 6);

    bool thrown = false;
    try {
        (void)cp_stress_gen::Math::sieve_spf(-1);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::factorize_spf(21, spf);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::factorize_spf(6, std::vector<int>{0, 1, 2, 3, 5, 5, 5});
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::euler_phi(0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::mod_add(1, 2, 0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::mod_mul(1, 2, 0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
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
        (void)cp_stress_gen::Math::crt_pair(1, 0, 2, 3);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::binom_small(-1, 0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Math::factorials_mod(-1, 5);
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
        (void)cp_stress_gen::Math::random_prime_sieve(14, 16, rng);
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

    thrown = false;
    try {
        (void)cp_stress_gen::Math::random_number_with_many_divisors(0, rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "[PASS] test_math\n";
    return 0;
}
