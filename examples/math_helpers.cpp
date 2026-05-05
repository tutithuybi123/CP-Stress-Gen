#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Printer out(std::cout);

    const long long power = cp_stress_gen::Math::mod_pow(2, 20, 1000000007);
    const auto eg = cp_stress_gen::Math::extended_gcd(30, 18);
    const long long inverse = cp_stress_gen::Math::mod_inverse(3, 11);
    const auto factors = cp_stress_gen::Math::factorize_trial(360);

    out.line(power);
    out.line(eg.gcd, eg.x, eg.y);
    out.line(inverse);
    out.line(factors.size());
    for (const auto& factor : factors) {
        out.line(factor.first, factor.second);
    }
    return 0;
}
