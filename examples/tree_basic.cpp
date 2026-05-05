#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(42);
    const int n = 8;
    const auto edges = cp_stress_gen::Tree(n).weighted(1, 20).random().build(rng);

    cp_stress_gen::core::Printer out(std::cout);
    out.line(n);
    out.edges(edges);
    return 0;
}

