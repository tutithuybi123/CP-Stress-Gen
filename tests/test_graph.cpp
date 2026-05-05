#include "cp_stress_gen.hpp"

#include <cassert>
#include <set>
#include <stdexcept>
#include <utility>

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

int main() {
    cp_stress_gen::core::Random rng(3);

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

    const auto weighted = cp_stress_gen::Graph(5).edges(6).directed().weighted(2, 4).build(rng);
    assert(weighted.size() == 6);
    for (const auto& edge : weighted) {
        assert(edge.u != edge.v);
        assert(edge.weighted);
        assert(edge.w >= 2 && edge.w <= 4);
    }

    bool thrown = false;
    try {
        (void)cp_stress_gen::Graph(3).edges(4).undirected().no_multi_edges().build(rng);
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

    return 0;
}

