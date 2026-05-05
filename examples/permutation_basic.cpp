#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(42);
    const auto permutation = cp_stress_gen::Permutation(10).shuffle().build(rng);

    cp_stress_gen::core::Printer out(std::cout);
    out.line(permutation.size());
    out.vector(permutation);
    return 0;
}

