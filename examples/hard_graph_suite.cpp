#include "cp_stress_gen.hpp"

#include <iostream>
#include <vector>

void print_graph(cp_stress_gen::core::Printer& out, const int n, const std::vector<cp_stress_gen::Graph::Edge>& edges) {
    out.line(n, edges.size());
    out.edges(edges);
}

int main() {
    cp_stress_gen::core::Random rng(302);
    cp_stress_gen::core::Printer out(std::cout);

    out.line(4);
    print_graph(out, 10, cp_stress_gen::Graph(10).edges(9).connected_components(3).build(rng));
    print_graph(out, 10, cp_stress_gen::Graph(10).forest(4).build(rng));
    print_graph(out, 12, cp_stress_gen::Graph(12).edges(20).layered_dag(4).no_multi_edges().build(rng));
    print_graph(out, 7, cp_stress_gen::Graph(7).dense().build(rng));
    return 0;
}
