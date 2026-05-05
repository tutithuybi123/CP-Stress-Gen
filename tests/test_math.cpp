#include "cp_stress_gen.hpp"

#include <cassert>
#include <stdexcept>
#include <vector>

int main() {
    assert(cp_stress_gen::Math::gcd(48, 18) == 6);
    assert(cp_stress_gen::Math::gcd(-48, 18) == 6);
    assert(cp_stress_gen::Math::lcm(12, 18) == 36);
    assert(cp_stress_gen::Math::lcm(0, 18) == 0);

    assert((cp_stress_gen::Math::divisors(12) == std::vector<long long>{1, 2, 3, 4, 6, 12}));
    assert((cp_stress_gen::Math::primes_up_to(10) == std::vector<int>{2, 3, 5, 7}));

    cp_stress_gen::core::Random rng(6);
    for (int i = 0; i < 100; ++i) {
        const int prime = cp_stress_gen::Math::random_prime(10, 30, rng);
        assert(prime == 11 || prime == 13 || prime == 17 || prime == 19 || prime == 23 || prime == 29);
    }

    bool thrown = false;
    try {
        (void)cp_stress_gen::Math::random_prime(14, 16, rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    return 0;
}

