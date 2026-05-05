#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(42);

    const auto values = cp_stress_gen::anti::SortKiller::many_duplicates(10, 3, rng);
    const auto tree = cp_stress_gen::anti::TreeKiller::chain(6);
    const auto text = cp_stress_gen::anti::StringKiller::alternating(12);

    cp_stress_gen::core::Printer out(std::cout);
    out.vector(values);
    out.edges(tree);
    out.line(text);
    return 0;
}

