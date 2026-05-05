#include "cp_stress_gen.hpp"

#include <iostream>
#include <string>
#include <vector>

void print_edges(cp_stress_gen::core::Printer& out, const int n, const std::vector<cp_stress_gen::Graph::Edge>& edges) {
    out.line(n, edges.size());
    out.edges(edges);
}

void print_tree(cp_stress_gen::core::Printer& out, const int n, const std::vector<cp_stress_gen::Tree::Edge>& edges) {
    out.line(n);
    out.edges(edges);
}

int main() {
    cp_stress_gen::core::Random rng(303);
    cp_stress_gen::core::Printer out(std::cout);

    const auto reversed = cp_stress_gen::anti::SortKiller::reversed(8);
    const auto duplicates = cp_stress_gen::anti::SortKiller::many_duplicates(12, 3, rng);
    const auto chain = cp_stress_gen::anti::TreeKiller::chain(7);
    const auto dag_chain = cp_stress_gen::anti::GraphKiller::long_dag_chain(7);
    const auto strings = cp_stress_gen::anti::StringKiller::prefix_heavy(3, 8);

    out.line("sort-reversed");
    out.vector(reversed);
    out.line("sort-duplicates");
    out.vector(duplicates);
    out.line("tree-chain");
    print_tree(out, 7, chain);
    out.line("graph-dag-chain");
    print_edges(out, 7, dag_chain);
    out.line("strings");
    for (const std::string& value : strings) {
        out.line(value);
    }
    return 0;
}
