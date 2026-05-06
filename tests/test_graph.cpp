#include "cp_stress_gen.hpp"

#include <cassert>
#include <iostream>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

static std::pair<int, int> key(const cp_stress_gen::Graph::Edge& edge, const bool directed) {
    if (directed || edge.u < edge.v) {
        return std::make_pair(edge.u, edge.v);
    }
    return std::make_pair(edge.v, edge.u);
}

static void assert_no_duplicate_edges(const std::vector<cp_stress_gen::Graph::Edge>& edges, const bool directed) {
    std::set<std::pair<int, int>> seen;
    for (const auto& edge : edges) {
        assert(edge.u != edge.v);
        const auto inserted = seen.insert(key(edge, directed));
        assert(inserted.second);
    }
}

static bool same_edges(
    const std::vector<cp_stress_gen::Graph::Edge>& left,
    const std::vector<cp_stress_gen::Graph::Edge>& right
) {
    if (left.size() != right.size()) {
        return false;
    }
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (left[i].u != right[i].u || left[i].v != right[i].v || left[i].w != right[i].w || left[i].weighted != right[i].weighted) {
            return false;
        }
    }
    return true;
}

int main() {
    cp_stress_gen::core::Random rng(3);

    assert(cp_stress_gen::Graph(0).edges(0).build(rng).empty());
    assert(cp_stress_gen::Graph(0).forest(0).build(rng).empty());
    assert(cp_stress_gen::Graph(1).path().build(rng).empty());
    assert(cp_stress_gen::Graph(1).complete().build(rng).empty());

    const auto dag = cp_stress_gen::Graph(8).edges(12).directed().dag().no_multi_edges().build(rng);
    assert(dag.size() == 12);
    assert_no_duplicate_edges(dag, true);
    for (const auto& edge : dag) {
        assert(edge.u < edge.v);
    }

    const auto layered = cp_stress_gen::Graph(9).edges(10).layered_dag(3).no_multi_edges().build(rng);
    assert(layered.size() == 10);
    assert_no_duplicate_edges(layered, true);
    for (const auto& edge : layered) {
        assert(edge.u < edge.v);
        const int source_layer = (edge.u - 1) * 3 / 9;
        const int target_layer = (edge.v - 1) * 3 / 9;
        assert(source_layer < target_layer);
    }

    const auto complete = cp_stress_gen::Graph(5).complete().build(rng);
    assert(complete.size() == 10);
    assert_no_duplicate_edges(complete, false);

    const auto cycle = cp_stress_gen::Graph(6).cycle().build(rng);
    assert(cycle.size() == 6);
    assert_no_duplicate_edges(cycle, false);

    const auto bipartite = cp_stress_gen::Graph(6).edges(8).bipartite(2).no_multi_edges().build(rng);
    assert(bipartite.size() == 8);
    assert_no_duplicate_edges(bipartite, false);
    for (const auto& edge : bipartite) {
        assert(edge.u >= 1 && edge.u <= 2);
        assert(edge.v >= 3 && edge.v <= 6);
    }

    const auto connected = cp_stress_gen::Graph(7).edges(8).sparse_connected().build(rng);
    assert(connected.size() == 8);
    assert_no_duplicate_edges(connected, false);

    const auto path = cp_stress_gen::Graph(6).path().build(rng);
    assert(path.size() == 5);
    for (std::size_t i = 0; i < path.size(); ++i) {
        assert(path[i].u + 1 == path[i].v);
    }

    const auto forest = cp_stress_gen::Graph(8).forest(3).build(rng);
    assert(forest.size() == 5);
    assert_no_duplicate_edges(forest, false);

    const auto components = cp_stress_gen::Graph(8).edges(6).connected_components(3).build(rng);
    assert(components.size() == 6);
    assert_no_duplicate_edges(components, false);

    const auto weighted = cp_stress_gen::Graph(5).edges(6).directed().weighted(2, 4).build(rng);
    assert(weighted.size() == 6);
    for (const auto& edge : weighted) {
        assert(edge.u != edge.v);
        assert(edge.weighted);
        assert(edge.w >= 2 && edge.w <= 4);
    }

    const auto wheel = cp_stress_gen::Graph(6).wheel().build(rng);
    assert(wheel.size() == 10);
    assert_no_duplicate_edges(wheel, false);

    const auto grid = cp_stress_gen::Graph(6).grid(2, 3).build(rng);
    assert(grid.size() == 7);
    assert_no_duplicate_edges(grid, false);

    const auto complete_bipartite = cp_stress_gen::Graph(7).complete_bipartite(3, 4).build(rng);
    assert(complete_bipartite.size() == 12);
    assert_no_duplicate_edges(complete_bipartite, false);
    for (const auto& edge : complete_bipartite) {
        assert(edge.u >= 1 && edge.u <= 3);
        assert(edge.v >= 4 && edge.v <= 7);
    }

    const auto tournament = cp_stress_gen::Graph(5).tournament().build(rng);
    assert(tournament.size() == 10);
    assert_no_duplicate_edges(tournament, true);
    std::set<std::pair<int, int>> unordered_pairs;
    for (const auto& edge : tournament) {
        unordered_pairs.insert(key(edge, false));
    }
    assert(unordered_pairs.size() == 10);

    const auto from_edges = cp_stress_gen::Graph::from_edges(4, std::vector<cp_stress_gen::Graph::Edge>{
        cp_stress_gen::Graph::Edge{1, 2, 1, false},
        cp_stress_gen::Graph::Edge{2, 3, 1, false},
        cp_stress_gen::Graph::Edge{3, 4, 1, false}
    }).build();
    assert(from_edges.size() == 3);
    assert_no_duplicate_edges(from_edges, false);

    const auto built_path = cp_stress_gen::GraphBuilder(4).add_path({1, 2, 3, 4}).build();
    assert(built_path.size() == 3);

    const auto built_cycle = cp_stress_gen::GraphBuilder(4).add_cycle({1, 2, 3, 4}).build();
    assert(built_cycle.size() == 4);
    assert_no_duplicate_edges(built_cycle, false);

    const auto clique = cp_stress_gen::GraphBuilder(4).add_clique({1, 2, 3, 4}).build();
    assert(clique.size() == 6);
    assert_no_duplicate_edges(clique, false);

    const auto directed_clique = cp_stress_gen::GraphBuilder(3).directed().add_clique({1, 2, 3}).build();
    assert(directed_clique.size() == 6);
    assert_no_duplicate_edges(directed_clique, true);

    const auto builder_bipartite = cp_stress_gen::GraphBuilder(5).add_bipartite({1, 2}, std::vector<int>{3, 4, 5}).build();
    assert(builder_bipartite.size() == 6);
    assert_no_duplicate_edges(builder_bipartite, false);

    auto merged_builder = cp_stress_gen::GraphBuilder(3).add_path({1, 2, 3});
    merged_builder.merge(cp_stress_gen::GraphBuilder(2).add_edge(1, 2));
    const auto merged_graph = merged_builder.build();
    assert(merged_graph.size() == 3);
    assert_no_duplicate_edges(merged_graph, false);

    auto shuffled_builder = cp_stress_gen::GraphBuilder(5).add_cycle({1, 2, 3, 4, 5});
    shuffled_builder.shuffle_vertices(rng).shuffle_edges(rng);
    const auto shuffled_graph = shuffled_builder.build();
    assert(shuffled_graph.size() == 5);
    assert_no_duplicate_edges(shuffled_graph, false);

    const auto deduped = cp_stress_gen::Graph::from_edges(3, std::vector<cp_stress_gen::Graph::Edge>{
        cp_stress_gen::Graph::Edge{1, 2, 1, false},
        cp_stress_gen::Graph::Edge{2, 1, 1, false},
        cp_stress_gen::Graph::Edge{2, 3, 1, false}
    }).remove_duplicate_edges().build();
    assert(deduped.size() == 2);
    assert_no_duplicate_edges(deduped, false);

    const auto complement = cp_stress_gen::GraphBuilder(4).add_path({1, 2, 3, 4}).complement().build();
    assert(complement.size() == 3);
    assert_no_duplicate_edges(complement, false);

    const auto directed_complement = cp_stress_gen::GraphBuilder(3, true).add_edge(1, 2).complement().build();
    assert(directed_complement.size() == 5);
    assert_no_duplicate_edges(directed_complement, true);

    cp_stress_gen::core::Random same_a(33);
    cp_stress_gen::core::Random same_b(33);
    assert(same_edges(
        cp_stress_gen::Graph(8).edges(7).connected_components(2).weighted(1, 5).shuffle().build(same_a),
        cp_stress_gen::Graph(8).edges(7).connected_components(2).weighted(1, 5).shuffle().build(same_b)
    ));

    bool thrown = false;
    try {
        (void)cp_stress_gen::Graph(3).wheel().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(5).wheel().directed().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(5).grid(2, 3).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(4).grid(0, 4).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(5).complete_bipartite(2, 2).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(4).tournament().undirected().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(4).edges(5).tournament().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(2).edges(1).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(!thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(0).edges(1).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(3).edges(7).directed().no_multi_edges().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(3).edges(4).undirected().no_multi_edges().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(3).edges(4).dag().no_multi_edges().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(3).layered_dag(1).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(5).bipartite(5).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(4).edges(5).bipartite(2).no_multi_edges().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(4).edges(5).complete().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(4).edges(3).cycle().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(2).cycle().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(4).edges(2).path().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(5).forest(0).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(0).forest(1).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(5).edges(4).forest(2).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(5).forest(2).directed().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(5).edges(1).connected_components(2).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(5).edges(99).connected_components(2).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph(5).connected_components(2).directed().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::GraphBuilder(2).add_edge(1, 1);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::GraphBuilder(3).add_edge(1, 2).add_edge(2, 1).build();
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::GraphBuilder(2).add_edge(1, 2, 5).complement();
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Graph::from_edges(3, std::vector<cp_stress_gen::Graph::Edge>{
            cp_stress_gen::Graph::Edge{1, 2, 1, false},
            cp_stress_gen::Graph::Edge{2, 1, 1, false}
        }).complement();
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "[PASS] test_graph\n";
    return 0;
}
