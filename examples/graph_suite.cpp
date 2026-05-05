#include "cp_stress_gen.hpp"

#include <iostream>

void print_graph(cp_stress_gen::core::Printer& out, const int n, const std::vector<cp_stress_gen::Graph::Edge>& edges) {
    out.line(n, edges.size());
    out.edges(edges);
}

int main() {
    cp_stress_gen::core::Random rng(42);
    cp_stress_gen::core::Printer out(std::cout);

    const auto connected = cp_stress_gen::Graph(8).edges(9).sparse_connected().build(rng);
    const auto dag = cp_stress_gen::Graph(8).edges(12).directed().dag().no_multi_edges().build(rng);
    const auto bipartite = cp_stress_gen::Graph(8).edges(10).bipartite(3).no_multi_edges().build(rng);
    const auto dense = cp_stress_gen::Graph(6).dense().build(rng);

    out.line(4);
    print_graph(out, 8, connected);
    print_graph(out, 8, dag);
    print_graph(out, 8, bipartite);
    print_graph(out, 6, dense);
    return 0;
}

