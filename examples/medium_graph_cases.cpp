#include "cp_stress_gen.hpp"

#include <iostream>
#include <vector>

void print_graph(cp_stress_gen::core::Printer& out, const int n, const std::vector<cp_stress_gen::Graph::Edge>& edges) {
    out.line(n, edges.size());
    out.edges(edges);
}

int main() {
    cp_stress_gen::core::Random rng(201);
    cp_stress_gen::core::Printer out(std::cout);

    out.line(4);
    print_graph(out, 7, cp_stress_gen::Graph(7).path().build(rng));
    print_graph(out, 7, cp_stress_gen::Graph(7).edges(8).sparse_connected().build(rng));
    print_graph(out, 7, cp_stress_gen::Graph(7).edges(9).dag().no_multi_edges().build(rng));
    print_graph(out, 8, cp_stress_gen::Graph(8).edges(10).bipartite(3).no_multi_edges().build(rng));
    return 0;
}
