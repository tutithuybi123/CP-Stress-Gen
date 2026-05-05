#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(101);
    cp_stress_gen::core::Printer out(std::cout);

    const int n = 10;
    const auto values = cp_stress_gen::Array(n).range(1, 50).build(rng);

    out.line(n);
    out.vector(values);
    return 0;
}
