#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(42);
    cp_stress_gen::core::Printer out(std::cout);

    const int tests = 5;
    out.line(tests);
    for (int tc = 1; tc <= tests; ++tc) {
        const int n = 5 + tc;
        const auto values = cp_stress_gen::Array(static_cast<std::size_t>(n)).range(1, 100).build(rng);
        out.line(n);
        out.vector(values);
    }
    return 0;
}

