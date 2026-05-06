#include "cp_stress_gen.hpp"

#include <iostream>
#include <vector>

int main() {
    cp_stress_gen::core::Random rng(77);
    cp_stress_gen::core::Printer out(std::cout);

    auto builder = cp_stress_gen::GraphBuilder(6)
        .add_path({1, 2, 3, 4})
        .add_clique({4, 5, 6})
        .remove_duplicate_edges();
    builder.shuffle_vertices(rng).shuffle_edges(rng);
    out.edges(builder.nodes(), builder.build());

    const auto complement = cp_stress_gen::GraphBuilder(5)
        .add_cycle({1, 2, 3, 4, 5})
        .complement()
        .build();
    out.edges(5, complement);

    const auto directed = cp_stress_gen::GraphBuilder(4, true)
        .add_bipartite(std::vector<int>{1, 2}, std::vector<int>{3, 4})
        .build();
    out.edges(4, directed);

    return 0;
}
