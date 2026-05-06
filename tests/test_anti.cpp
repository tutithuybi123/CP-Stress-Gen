#include "cp_stress_gen.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
    cp_stress_gen::core::Random rng(8);

    assert((cp_stress_gen::anti::SortKiller::reversed(5) == std::vector<long long>{5, 4, 3, 2, 1}));

    auto nearly = cp_stress_gen::anti::SortKiller::nearly_sorted(10, 3, rng);
    std::sort(nearly.begin(), nearly.end());
    assert((nearly == std::vector<long long>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));

    const auto duplicates = cp_stress_gen::anti::SortKiller::many_duplicates(20, 3, rng);
    assert(duplicates.size() == 20);
    for (const auto value : duplicates) {
        assert(value >= 1 && value <= 3);
    }
    assert((cp_stress_gen::anti::SortKiller::organ_pipe(7) == std::vector<long long>{1, 2, 3, 4, 3, 2, 1}));
    assert((cp_stress_gen::anti::SortKiller::alternating_high_low(6) == std::vector<long long>{1, 6, 2, 5, 3, 4}));
    assert((cp_stress_gen::anti::SortKiller::quicksort_bad_candidate(5) == std::vector<long long>{1, 2, 3, 2, 1}));

    assert(cp_stress_gen::anti::TreeKiller::chain(6).size() == 5);
    assert(cp_stress_gen::anti::TreeKiller::star(6).size() == 5);
    assert(cp_stress_gen::anti::TreeKiller::chain_with_branches(6, 2, rng).size() == 5);
    const auto broom = cp_stress_gen::anti::TreeKiller::broom(7, 4);
    assert(broom.size() == 6);
    assert(broom[0].u == 1 && broom[0].v == 2);
    assert(broom[3].u == 4 && broom[3].v == 5);

    assert(cp_stress_gen::anti::GraphKiller::dense(5, rng).size() == 8);
    const auto dag_chain = cp_stress_gen::anti::GraphKiller::long_dag_chain(5);
    assert(dag_chain.size() == 4);
    for (const auto& edge : dag_chain) {
        assert(edge.u + 1 == edge.v);
    }
    assert(cp_stress_gen::anti::GraphKiller::sparse_connected(5, rng).size() == 4);
    const auto dense_dag = cp_stress_gen::anti::GraphKiller::dense_dag(4);
    assert(dense_dag.size() == 6);
    for (const auto& edge : dense_dag) {
        assert(edge.u < edge.v);
    }

    assert(cp_stress_gen::anti::StringKiller::repeated(4, 'x') == "xxxx");
    assert(cp_stress_gen::anti::StringKiller::alternating(5, 'a', 'b') == "ababa");
    assert(cp_stress_gen::anti::StringKiller::kmp_prefix_pattern(6) == "aaaaab");
    const auto prefix = cp_stress_gen::anti::StringKiller::prefix_heavy(3, 5);
    assert(prefix.size() == 3);
    for (const auto& value : prefix) {
        assert(value.size() == 5);
        assert(value.substr(0, 4) == "aaaa");
    }

    assert(cp_stress_gen::anti::DfsBfsKiller::deep_chain(8).size() == 7);
    assert(cp_stress_gen::anti::DfsBfsKiller::many_components(8, 3).size() == 5);
    assert(cp_stress_gen::anti::DfsBfsKiller::huge_branching(8).size() == 7);
    assert(cp_stress_gen::anti::DfsBfsKiller::broom_tree(8, 5).size() == 7);
    assert(cp_stress_gen::anti::DfsBfsKiller::grid_maze_like(2, 3).size() == 7);

    const auto equal_distances = cp_stress_gen::anti::DijkstraKiller::many_equal_distances(5, 4);
    assert(equal_distances.size() == 4);
    for (const auto& edge : equal_distances) {
        assert(edge.weighted && edge.w == 1);
    }
    const auto zero_edges = cp_stress_gen::anti::DijkstraKiller::zero_weight_edges(5, 4);
    assert(zero_edges.size() == 4);
    for (const auto& edge : zero_edges) {
        assert(edge.weighted && edge.w == 0);
    }
    assert(cp_stress_gen::anti::DijkstraKiller::dense_weighted_trap(5).size() == 8);
    const auto layered_paths = cp_stress_gen::anti::DijkstraKiller::layered_equal_shortest_paths(3, 2);
    assert(layered_paths.size() == 8);
    for (const auto& edge : layered_paths) {
        assert(edge.weighted && edge.w == 1 && edge.u < edge.v);
    }

    assert(cp_stress_gen::anti::DsuKiller::redundant_edges(5, 3).size() == 7);
    assert(cp_stress_gen::anti::DsuKiller::delayed_connectivity(5).front().u == 4);
    const auto reverse_unions = cp_stress_gen::anti::DsuKiller::reverse_union_sequence(5);
    assert(reverse_unions.size() == 4);
    assert(reverse_unions.front().type == cp_stress_gen::anti::DsuQuery::Unite);
    const auto same_queries = cp_stress_gen::anti::DsuKiller::many_queries_same_component(4, 3);
    assert(same_queries.size() == 6);
    assert(same_queries.back().type == cp_stress_gen::anti::DsuQuery::Same);

    assert((cp_stress_gen::anti::DpKiller::all_equal_array(4, 9) == std::vector<long long>{9, 9, 9, 9}));
    assert((cp_stress_gen::anti::DpKiller::monotonic_array(4) == std::vector<long long>{1, 2, 3, 4}));
    assert((cp_stress_gen::anti::DpKiller::alternating_array(5, 1, 7) == std::vector<long long>{1, 7, 1, 7, 1}));
    assert((cp_stress_gen::anti::DpKiller::boundary_sizes(3) == std::vector<std::size_t>{0, 1, 2, 3}));
    const auto items = cp_stress_gen::anti::DpKiller::knapsack_tight_capacity(4, 10);
    assert(items.size() == 4);
    for (const auto& item : items) {
        assert(item.weight > 0 && item.value == item.weight);
    }

    assert((cp_stress_gen::anti::GreedyKiller::local_optimum_trap_array(5) == std::vector<long long>{2, 1, 4, 3, 5}));
    const auto intervals = cp_stress_gen::anti::GreedyKiller::interval_scheduling_trap(4);
    assert(intervals.size() == 4);
    assert(intervals.front().l == 1 && intervals.front().r == 5);
    assert((cp_stress_gen::anti::GreedyKiller::coin_change_greedy_trap() == std::vector<int>{1, 3, 4}));

    assert((cp_stress_gen::anti::BinarySearchKiller::off_by_one_boundaries(5) == std::vector<long long>{0, 4, 5}));
    assert((cp_stress_gen::anti::BinarySearchKiller::lower_upper_bound_duplicates(5) == std::vector<long long>{1, 1, 2, 2, 2}));
    assert((cp_stress_gen::anti::BinarySearchKiller::all_false_then_true(5, 3) == std::vector<int>{0, 0, 0, 1, 1}));
    assert((cp_stress_gen::anti::BinarySearchKiller::all_true_then_false(5, 3) == std::vector<int>{1, 1, 1, 0, 0}));

    assert(cp_stress_gen::anti::StringPatternKiller::kmp_worst_prefix(5) == "aaaab");
    assert(cp_stress_gen::anti::StringPatternKiller::z_repeated_blocks(4) == "aaaa");
    assert(cp_stress_gen::anti::StringPatternKiller::border_heavy(4) == "aaaa");
    assert(cp_stress_gen::anti::StringPatternKiller::periodic_with_break(5) == "ababc");
    assert(cp_stress_gen::anti::HashKiller::repeated_base_sensitive_shapes(4) == "azaz");
    const auto collision_like = cp_stress_gen::anti::HashKiller::collision_like_patterns(4);
    assert(collision_like.first == "aaab");
    assert(collision_like.second == "baaa");

    bool thrown = false;
    try {
        (void)cp_stress_gen::anti::SortKiller::many_duplicates(5, 0, rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::anti::TreeKiller::broom(5, 0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::anti::DijkstraKiller::layered_equal_shortest_paths(1, 2);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::anti::BinarySearchKiller::all_false_then_true(3, 4);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "[PASS] test_anti\n";
    return 0;
}
