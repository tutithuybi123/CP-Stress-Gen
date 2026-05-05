#include "cp_stress_gen.hpp"

#include <cassert>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>

class Dsu {
public:
    explicit Dsu(const int n) : parent_(n), rank_(n, 0) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    int find(const int x) {
        if (parent_[x] == x) {
            return x;
        }
        parent_[x] = find(parent_[x]);
        return parent_[x];
    }

    void unite(const int a, const int b) {
        int x = find(a);
        int y = find(b);
        if (x == y) {
            return;
        }
        if (rank_[x] < rank_[y]) {
            std::swap(x, y);
        }
        parent_[y] = x;
        if (rank_[x] == rank_[y]) {
            ++rank_[x];
        }
    }

private:
    std::vector<int> parent_;
    std::vector<int> rank_;
};

static void assert_tree(const std::vector<cp_stress_gen::Tree::Edge>& edges, const int n, const int first) {
    assert(edges.size() == static_cast<std::size_t>(n > 0 ? n - 1 : 0));
    Dsu dsu(n);
    for (const auto& edge : edges) {
        assert(edge.u >= first && edge.u < first + n);
        assert(edge.v >= first && edge.v < first + n);
        assert(edge.u != edge.v);
        dsu.unite(edge.u - first, edge.v - first);
    }
    for (int i = 1; i < n; ++i) {
        assert(dsu.find(0) == dsu.find(i));
    }
}

static bool same_edges(
    const std::vector<cp_stress_gen::Tree::Edge>& left,
    const std::vector<cp_stress_gen::Tree::Edge>& right
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

static void assert_max_degree(const std::vector<cp_stress_gen::Tree::Edge>& edges, const int n, const int first, const int limit) {
    std::vector<int> degree(static_cast<std::size_t>(n), 0);
    for (const auto& edge : edges) {
        ++degree[static_cast<std::size_t>(edge.u - first)];
        ++degree[static_cast<std::size_t>(edge.v - first)];
    }
    for (const int value : degree) {
        assert(value <= limit);
    }
}

int main() {
    cp_stress_gen::core::Random rng(2);

    assert_tree(cp_stress_gen::Tree(0).bamboo().build(rng), 0, 1);
    assert_tree(cp_stress_gen::Tree(1).star().build(rng), 1, 1);
    assert_tree(cp_stress_gen::Tree(10).bamboo().build(rng), 10, 1);
    assert_tree(cp_stress_gen::Tree(10).star(3).build(rng), 10, 1);
    assert_tree(cp_stress_gen::Tree(10).random().build(rng), 10, 1);
    assert_tree(cp_stress_gen::Tree(10).binary().build(rng), 10, 1);
    assert_tree(cp_stress_gen::Tree(10).caterpillar(4).build(rng), 10, 1);
    assert_tree(cp_stress_gen::Tree(10).deep_recursion(3).build(rng), 10, 1);

    const auto weighted = cp_stress_gen::Tree(12).zero_based().weighted(5, 8).random().shuffle().build(rng);
    assert_tree(weighted, 12, 0);
    for (const auto& edge : weighted) {
        assert(edge.weighted);
        assert(edge.w >= 5 && edge.w <= 8);
    }

    const auto limited = cp_stress_gen::Tree(20).zero_based().weighted(1, 3).random().degree_limit(3).build(rng);
    assert_tree(limited, 20, 0);
    assert_max_degree(limited, 20, 0, 3);
    for (const auto& edge : limited) {
        assert(edge.weighted);
        assert(edge.w >= 1 && edge.w <= 3);
    }

    cp_stress_gen::core::Random same_a(44);
    cp_stress_gen::core::Random same_b(44);
    assert(same_edges(
        cp_stress_gen::Tree(15).weighted(2, 9).random().shuffle().build(same_a),
        cp_stress_gen::Tree(15).weighted(2, 9).random().shuffle().build(same_b)
    ));

    bool thrown = false;
    try {
        (void)cp_stress_gen::Tree(5).random().degree_limit(1).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Tree(5).bamboo().degree_limit(2).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Tree(5).degree_limit(0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Tree(4).star(10).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Tree(4).star(0).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Tree(4).weighted(9, 2);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Tree(4).caterpillar(0).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Tree(4).caterpillar(5).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Tree(4).deep_recursion(4).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Tree(4).deep_recursion(5).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "[PASS] test_tree\n";
    return 0;
}
