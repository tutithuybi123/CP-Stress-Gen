#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(42);
    const auto values = cp_stress_gen::Array(5).range(1, 9).build(rng);

    cp_stress_gen::core::Printer out(std::cout);
    out.with_n(values);

    const auto edges = cp_stress_gen::Tree(4).bamboo().build(rng);
    out.edges(4, edges);

    return 0;
}
