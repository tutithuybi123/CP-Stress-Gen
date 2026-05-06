#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Printer out(std::cout);

    const auto dfs_tree = cp_stress_gen::anti::DfsBfsKiller::broom_tree(10, 7);
    out.edges(10, dfs_tree);

    const auto dijkstra = cp_stress_gen::anti::DijkstraKiller::layered_equal_shortest_paths(3, 3);
    out.edges(9, dijkstra);

    const auto dsu_queries = cp_stress_gen::anti::DsuKiller::many_queries_same_component(5, 3);
    out.line(dsu_queries.size());
    for (const auto& query : dsu_queries) {
        out.line(query.type == cp_stress_gen::anti::DsuQuery::Unite ? "union" : "same", query.u, query.v);
    }

    out.vector(cp_stress_gen::anti::DpKiller::alternating_array(8, 1, 100));
    out.vector(cp_stress_gen::anti::BinarySearchKiller::lower_upper_bound_duplicates(10));
    out.line(cp_stress_gen::anti::StringPatternKiller::periodic_with_break(12));

    return 0;
}
