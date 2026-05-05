#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(42);
    const auto values = cp_stress_gen::Array(10).range(1, 100).build(rng);

    cp_stress_gen::core::Printer out(std::cout);
    out.line(values.size());
    out.vector(values);
    return 0;
}

