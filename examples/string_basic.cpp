#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(42);
    const auto value = cp_stress_gen::String(16).lowercase().build(rng);

    cp_stress_gen::core::Printer out(std::cout);
    out.line(value);
    return 0;
}

