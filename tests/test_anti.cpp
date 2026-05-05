#include "cp_stress_gen.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
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

    std::cout << "[PASS] test_anti\n";
    return 0;
}
