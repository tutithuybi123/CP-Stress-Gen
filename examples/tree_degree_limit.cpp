#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(401);
    cp_stress_gen::core::Printer out(std::cout);

    const int n = 12;
    const auto edges = cp_stress_gen::Tree(n)
        .random()
        .degree_limit(3)
        .weighted(1, 20)
        .build(rng);

    out.line(n);
    out.edges(edges);
    return 0;
}
