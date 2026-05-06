#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(123);
    cp_stress_gen::core::Printer out(std::cout);

    auto base = cp_stress_gen::TreeBuilder::bamboo(5);
    base.attach(cp_stress_gen::TreeBuilder::star(4), 5, 1)
        .shuffle_vertices(rng)
        .shuffle_edges(rng);

    const auto composed = base.build();
    out.edges(base.nodes(), composed);

    const auto broom = cp_stress_gen::Tree(8).broom(5, 3).build(rng);
    out.edges(8, broom);

    const auto kary = cp_stress_gen::TreeBuilder::k_ary(10, 3).zero_based().build();
    out.indexed(1).edges(10, kary);

    return 0;
}
