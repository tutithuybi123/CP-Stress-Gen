#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(2026);
    cp_stress_gen::core::Printer out(std::cout);

    const auto spf = cp_stress_gen::Math::sieve_spf(100);
    const auto factors = cp_stress_gen::Math::factorize_spf(84, spf);
    const auto crt = cp_stress_gen::Math::crt_pair(2, 3, 3, 5);
    const auto factorials = cp_stress_gen::Math::factorials_mod(6, 1000000007);
    const int rich = cp_stress_gen::Math::random_number_with_many_divisors(100, rng);

    out.line("factorization of 84");
    for (const auto& factor : factors) {
        out.line(factor.first, factor.second);
    }

    out.line("phi(84)", cp_stress_gen::Math::euler_phi(84));
    out.line("binom(10,3)", cp_stress_gen::Math::binom_small(10, 3));
    out.line("crt", crt.compatible, crt.value, crt.mod);
    out.line("random divisor-rich number", rich);
    out.vector(factorials);

    return 0;
}
