#include "cp_stress_gen.hpp"

#include <cassert>
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

int main() {
    cp_stress_gen::core::Random rng(2);

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

    bool thrown = false;
    try {
        (void)cp_stress_gen::Tree(4).star(10).build(rng);
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
        (void)cp_stress_gen::Tree(4).deep_recursion(4).build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    return 0;
}

