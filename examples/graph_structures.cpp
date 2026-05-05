#include "cp_stress_gen.hpp"

#include <iostream>
#include <vector>

void print_graph(cp_stress_gen::core::Printer& out, const int n, const std::vector<cp_stress_gen::Graph::Edge>& edges) {
    out.line(n, edges.size());
    out.edges(edges);
}

int main() {
    cp_stress_gen::core::Random rng(402);
    cp_stress_gen::core::Printer out(std::cout);

    out.line(4);
    print_graph(out, 6, cp_stress_gen::Graph(6).wheel().build(rng));
    print_graph(out, 9, cp_stress_gen::Graph(9).grid(3, 3).build(rng));
    print_graph(out, 7, cp_stress_gen::Graph(7).complete_bipartite(3, 4).build(rng));
    print_graph(out, 6, cp_stress_gen::Graph(6).tournament().build(rng));
    return 0;
}
