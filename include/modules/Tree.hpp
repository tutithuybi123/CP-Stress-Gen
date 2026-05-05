#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

namespace cp_stress_gen {

class Tree {
public:
    using node_type = int;
    using weight_type = long long;
    using size_type = std::size_t;

    struct Edge {
        node_type u;
        node_type v;
        weight_type w;
        bool weighted;
    };

    using edge_type = Edge;

    explicit Tree(const size_type nodes) : nodes_(nodes) {}

    Tree& first_node(const node_type first) noexcept {
        first_ = first;
        return *this;
    }

    Tree& one_based() noexcept {
        return first_node(1);
    }

    Tree& zero_based() noexcept {
        return first_node(0);
    }

    Tree& weight(const weight_type value) noexcept {
        weighted_ = true;
        weight_left_ = value;
        weight_right_ = value;
        return *this;
    }

    Tree& weighted(const weight_type left, const weight_type right) {
        core::require_range(left, right, "Tree::weighted requires left <= right");
        weighted_ = true;
        weight_left_ = left;
        weight_right_ = right;
        return *this;
    }

    Tree& bamboo() noexcept {
        mode_ = Mode::Bamboo;
        return *this;
    }

    Tree& star() noexcept {
        mode_ = Mode::Star;
        has_center_ = false;
        return *this;
    }

    Tree& star(const node_type center) noexcept {
        mode_ = Mode::Star;
        center_ = center;
        has_center_ = true;
        return *this;
    }

    Tree& random() noexcept {
        mode_ = Mode::Random;
        return *this;
    }

    Tree& degree_limit(const size_type limit) {
        core::require_positive(limit, "Tree::degree_limit limit must be positive");
        has_degree_limit_ = true;
        degree_limit_ = limit;
        return *this;
    }

    Tree& binary() noexcept {
        mode_ = Mode::Binary;
        return *this;
    }

    Tree& caterpillar(const size_type spine) noexcept {
        mode_ = Mode::Caterpillar;
        spine_ = spine;
        return *this;
    }

    Tree& deep_recursion(const size_type branches = 0) noexcept {
        mode_ = Mode::DeepRecursion;
        branches_ = branches;
        return *this;
    }

    Tree& shuffle() noexcept {
        shuffle_ = true;
        return *this;
    }

    [[nodiscard]] size_type nodes() const noexcept {
        return nodes_;
    }

    [[nodiscard]] std::vector<edge_type> build(core::Random& rng) const {
        validate_common();

        std::vector<edge_type> result;
        if (nodes_ < 2) {
            return result;
        }

        result.reserve(nodes_ - 1);
        if (mode_ == Mode::Bamboo) {
            generate_bamboo(result, rng);
        } else if (mode_ == Mode::Star) {
            generate_star(result, rng);
        } else if (mode_ == Mode::Random) {
            generate_random(result, rng);
        } else if (mode_ == Mode::Binary) {
            generate_binary(result, rng);
        } else if (mode_ == Mode::Caterpillar) {
            generate_caterpillar(result, rng);
        } else {
            generate_deep_recursion(result, rng);
        }

        if (shuffle_) {
            shuffle_edges(result, rng);
        }
        return result;
    }

    [[nodiscard]] std::vector<edge_type> build() const {
        core::Random rng = core::Random::from_time();
        return build(rng);
    }

private:
    enum class Mode {
        Bamboo,
        Star,
        Random,
        Binary,
        Caterpillar,
        DeepRecursion
    };

    size_type nodes_;
    node_type first_{1};
    node_type center_{1};
    bool has_center_{false};
    bool weighted_{false};
    weight_type weight_left_{1};
    weight_type weight_right_{1};
    size_type spine_{0};
    size_type branches_{0};
    size_type degree_limit_{0};
    bool has_degree_limit_{false};
    bool shuffle_{false};
    Mode mode_{Mode::Bamboo};

    void validate_common() const {
        if (has_degree_limit_) {
            core::require(mode_ == Mode::Random, "Tree::degree_limit currently supports random trees only");
            core::require(nodes_ <= 2 || degree_limit_ >= 2, "Tree::degree_limit needs limit >= 2 when n > 2");
        }
        if (nodes_ == 0) {
            return;
        }
        core::require(nodes_ <= static_cast<size_type>(2147483647), "Tree node count exceeds int label capacity");
        (void)label(nodes_ - 1);
    }

    [[nodiscard]] node_type label(const size_type offset) const noexcept {
        return static_cast<node_type>(first_ + static_cast<node_type>(offset));
    }

    [[nodiscard]] weight_type next_weight(core::Random& rng) const {
        return weighted_ ? rng.integer<weight_type>(weight_left_, weight_right_) : weight_type{1};
    }

    [[nodiscard]] edge_type make_edge(const node_type u, const node_type v, core::Random& rng) const {
        return Edge{u, v, next_weight(rng), weighted_};
    }

    void generate_bamboo(std::vector<edge_type>& result, core::Random& rng) const {
        for (size_type i = 0; i + 1 < nodes_; ++i) {
            result.push_back(make_edge(label(i), label(i + 1), rng));
        }
    }

    void generate_star(std::vector<edge_type>& result, core::Random& rng) const {
        const node_type root = has_center_ ? center_ : first_;
        core::require(root >= first_ && root <= label(nodes_ - 1), "Tree::star center is outside node label range");

        for (size_type i = 0; i < nodes_; ++i) {
            const node_type v = label(i);
            if (v != root) {
                result.push_back(make_edge(root, v, rng));
            }
        }
    }

    void generate_random(std::vector<edge_type>& result, core::Random& rng) const {
        if (has_degree_limit_) {
            generate_degree_limited_random(result, rng);
            return;
        }
        for (size_type i = 1; i < nodes_; ++i) {
            const size_type parent = rng.integer<size_type>(0, i - 1);
            result.push_back(make_edge(label(parent), label(i), rng));
        }
    }

    void generate_degree_limited_random(std::vector<edge_type>& result, core::Random& rng) const {
        std::vector<size_type> degree(nodes_, 0);
        std::vector<size_type> candidates;
        candidates.reserve(nodes_);
        candidates.push_back(0);

        for (size_type i = 1; i < nodes_; ++i) {
            core::require(!candidates.empty(), "Tree::degree_limit cannot attach a new node");
            const size_type index = rng.integer<size_type>(0, candidates.size() - 1);
            const size_type parent = candidates[index];

            result.push_back(make_edge(label(parent), label(i), rng));
            ++degree[parent];
            ++degree[i];

            if (degree[parent] >= degree_limit_) {
                candidates[index] = candidates.back();
                candidates.pop_back();
            }
            if (degree[i] < degree_limit_) {
                candidates.push_back(i);
            }
        }
    }

    void generate_binary(std::vector<edge_type>& result, core::Random& rng) const {
        for (size_type i = 1; i < nodes_; ++i) {
            result.push_back(make_edge(label((i - 1) / 2), label(i), rng));
        }
    }

    void generate_caterpillar(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(spine_ > 0 && spine_ <= nodes_, "Tree::caterpillar spine must be in [1, n]");

        for (size_type i = 0; i + 1 < spine_; ++i) {
            result.push_back(make_edge(label(i), label(i + 1), rng));
        }
        for (size_type i = spine_; i < nodes_; ++i) {
            const size_type parent = rng.integer<size_type>(0, spine_ - 1);
            result.push_back(make_edge(label(parent), label(i), rng));
        }
    }

    void generate_deep_recursion(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(branches_ < nodes_, "Tree::deep_recursion branches must be less than n");
        const size_type chain_nodes = nodes_ - branches_;
        core::require(chain_nodes > 0, "Tree::deep_recursion needs at least one chain node");

        for (size_type i = 0; i + 1 < chain_nodes; ++i) {
            result.push_back(make_edge(label(i), label(i + 1), rng));
        }
        for (size_type i = chain_nodes; i < nodes_; ++i) {
            const size_type parent = rng.integer<size_type>(0, chain_nodes - 1);
            result.push_back(make_edge(label(parent), label(i), rng));
        }
    }

    static void shuffle_edges(std::vector<edge_type>& result, core::Random& rng) {
        if (result.size() < 2) {
            return;
        }

        for (size_type i = result.size() - 1; i > 0; --i) {
            const size_type j = rng.integer<size_type>(0, i);
            std::swap(result[i], result[j]);
        }
    }
};

} // namespace cp_stress_gen
