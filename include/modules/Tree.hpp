#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <algorithm>
#include <cstddef>
#include <set>
#include <utility>
#include <vector>

namespace cp_stress_gen {

class TreeBuilder;

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

    Tree& broom(const size_type chain_len, const size_type leaves) noexcept {
        mode_ = Mode::Broom;
        chain_len_ = chain_len;
        leaves_ = leaves;
        return *this;
    }

    Tree& double_star(const size_type left_leaves, const size_type right_leaves) noexcept {
        mode_ = Mode::DoubleStar;
        left_leaves_ = left_leaves;
        right_leaves_ = right_leaves;
        return *this;
    }

    Tree& k_ary(const size_type k) noexcept {
        mode_ = Mode::Kary;
        k_ = k;
        return *this;
    }

    Tree& shuffle() noexcept {
        shuffle_ = true;
        return *this;
    }

    [[nodiscard]] size_type nodes() const noexcept {
        return nodes_;
    }

    [[nodiscard]] static TreeBuilder from_edges(size_type n, const std::vector<edge_type>& edges, node_type first = 1);

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
        } else if (mode_ == Mode::DeepRecursion) {
            generate_deep_recursion(result, rng);
        } else if (mode_ == Mode::Broom) {
            generate_broom(result, rng);
        } else if (mode_ == Mode::DoubleStar) {
            generate_double_star(result, rng);
        } else {
            generate_k_ary(result, rng);
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
        DeepRecursion,
        Broom,
        DoubleStar,
        Kary
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
    size_type chain_len_{0};
    size_type leaves_{0};
    size_type left_leaves_{0};
    size_type right_leaves_{0};
    size_type k_{0};
    size_type degree_limit_{0};
    bool has_degree_limit_{false};
    bool shuffle_{false};
    Mode mode_{Mode::Bamboo};

    void validate_common() const {
        if (has_degree_limit_) {
            core::require(mode_ == Mode::Random, "Tree::degree_limit currently supports random trees only");
            core::require(nodes_ <= 2 || degree_limit_ >= 2, "Tree::degree_limit needs limit >= 2 when n > 2");
        }
        if (mode_ == Mode::Broom) {
            core::require(chain_len_ > 0, "Tree::broom chain length must be positive");
            core::require(chain_len_ + leaves_ == nodes_, "Tree::broom chain_len + leaves must equal n");
        }
        if (mode_ == Mode::DoubleStar) {
            core::require(nodes_ >= 2, "Tree::double_star needs at least two centers");
            core::require(left_leaves_ + right_leaves_ + 2 == nodes_, "Tree::double_star leaves + 2 must equal n");
        }
        if (mode_ == Mode::Kary) {
            core::require(k_ > 0, "Tree::k_ary k must be positive");
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

    void generate_broom(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(chain_len_ > 0, "Tree::broom chain length must be positive");
        core::require(chain_len_ + leaves_ == nodes_, "Tree::broom chain_len + leaves must equal n");

        for (size_type i = 0; i + 1 < chain_len_; ++i) {
            result.push_back(make_edge(label(i), label(i + 1), rng));
        }
        for (size_type i = chain_len_; i < nodes_; ++i) {
            result.push_back(make_edge(label(chain_len_ - 1), label(i), rng));
        }
    }

    void generate_double_star(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(nodes_ >= 2, "Tree::double_star needs at least two centers");
        core::require(left_leaves_ + right_leaves_ + 2 == nodes_, "Tree::double_star leaves + 2 must equal n");

        result.push_back(make_edge(label(0), label(1), rng));
        size_type next = 2;
        for (size_type i = 0; i < left_leaves_; ++i) {
            result.push_back(make_edge(label(0), label(next++), rng));
        }
        for (size_type i = 0; i < right_leaves_; ++i) {
            result.push_back(make_edge(label(1), label(next++), rng));
        }
    }

    void generate_k_ary(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(k_ > 0, "Tree::k_ary k must be positive");
        for (size_type i = 1; i < nodes_; ++i) {
            result.push_back(make_edge(label((i - 1) / k_), label(i), rng));
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

class TreeBuilder {
public:
    using node_type = Tree::node_type;
    using weight_type = Tree::weight_type;
    using size_type = Tree::size_type;
    using edge_type = Tree::edge_type;

    explicit TreeBuilder(const size_type nodes = 0, const node_type first = 1) : nodes_(nodes), first_(first) {
        validate_label_capacity();
    }

    [[nodiscard]] static TreeBuilder from_edges(const size_type n, const std::vector<edge_type>& edges, const node_type first = 1) {
        TreeBuilder builder(n, first);
        builder.edges_ = edges;
        builder.validate_tree();
        return builder;
    }

    [[nodiscard]] static TreeBuilder bamboo(const size_type n) {
        TreeBuilder builder(n);
        for (size_type i = 0; i + 1 < n; ++i) {
            builder.edges_.push_back(Edge(label_from(1, i), label_from(1, i + 1), 1, false));
        }
        return builder;
    }

    [[nodiscard]] static TreeBuilder star(const size_type n) {
        TreeBuilder builder(n);
        for (size_type i = 1; i < n; ++i) {
            builder.edges_.push_back(Edge(1, label_from(1, i), 1, false));
        }
        return builder;
    }

    [[nodiscard]] static TreeBuilder broom(const size_type chain_len, const size_type leaves) {
        core::require(chain_len > 0, "TreeBuilder::broom chain length must be positive");
        TreeBuilder builder(chain_len + leaves);
        for (size_type i = 0; i + 1 < chain_len; ++i) {
            builder.edges_.push_back(Edge(label_from(1, i), label_from(1, i + 1), 1, false));
        }
        for (size_type i = 0; i < leaves; ++i) {
            builder.edges_.push_back(Edge(label_from(1, chain_len - 1), label_from(1, chain_len + i), 1, false));
        }
        return builder;
    }

    [[nodiscard]] static TreeBuilder double_star(const size_type left_leaves, const size_type right_leaves) {
        TreeBuilder builder(left_leaves + right_leaves + 2);
        builder.edges_.push_back(Edge(1, 2, 1, false));
        size_type next = 2;
        for (size_type i = 0; i < left_leaves; ++i) {
            builder.edges_.push_back(Edge(1, label_from(1, next++), 1, false));
        }
        for (size_type i = 0; i < right_leaves; ++i) {
            builder.edges_.push_back(Edge(2, label_from(1, next++), 1, false));
        }
        return builder;
    }

    [[nodiscard]] static TreeBuilder k_ary(const size_type n, const size_type k) {
        core::require(k > 0, "TreeBuilder::k_ary k must be positive");
        TreeBuilder builder(n);
        for (size_type i = 1; i < n; ++i) {
            builder.edges_.push_back(Edge(label_from(1, (i - 1) / k), label_from(1, i), 1, false));
        }
        return builder;
    }

    TreeBuilder& attach(const TreeBuilder& child, const node_type parent_node, const node_type child_root_node) {
        validate_tree();
        child.validate_tree();
        core::require(contains(parent_node), "TreeBuilder::attach parent node is outside the builder label range");
        core::require(child.nodes_ > 0, "TreeBuilder::attach child must not be empty");
        core::require(child.contains(child_root_node), "TreeBuilder::attach child root is outside the child label range");

        const size_type original_nodes = nodes_;
        for (const auto& edge : child.edges_) {
            edges_.push_back(Edge(
                relabel_child(edge.u, child, original_nodes),
                relabel_child(edge.v, child, original_nodes),
                edge.w,
                edge.weighted
            ));
        }
        edges_.push_back(Edge(parent_node, relabel_child(child_root_node, child, original_nodes), 1, false));
        nodes_ += child.nodes_;
        validate_label_capacity();
        validate_tree();
        return *this;
    }

    TreeBuilder& merge(const TreeBuilder& other, const node_type parent_node, const node_type other_root_node) {
        return attach(other, parent_node, other_root_node);
    }

    TreeBuilder& renumber(const node_type first) {
        for (auto& edge : edges_) {
            edge.u = label_from(first, offset_of(edge.u));
            edge.v = label_from(first, offset_of(edge.v));
        }
        first_ = first;
        validate_label_capacity();
        return *this;
    }

    TreeBuilder& one_based() {
        return renumber(1);
    }

    TreeBuilder& zero_based() {
        return renumber(0);
    }

    TreeBuilder& shuffle_vertices(core::Random& rng) {
        if (nodes_ < 2) {
            return *this;
        }
        std::vector<node_type> labels;
        labels.reserve(nodes_);
        for (size_type i = 0; i < nodes_; ++i) {
            labels.push_back(label_from(first_, i));
        }
        for (size_type i = labels.size() - 1; i > 0; --i) {
            const size_type j = rng.integer<size_type>(0, i);
            std::swap(labels[i], labels[j]);
        }
        for (auto& edge : edges_) {
            edge.u = labels[offset_of(edge.u)];
            edge.v = labels[offset_of(edge.v)];
        }
        validate_tree();
        return *this;
    }

    TreeBuilder& shuffle_edges(core::Random& rng) {
        if (edges_.size() < 2) {
            return *this;
        }
        for (size_type i = edges_.size() - 1; i > 0; --i) {
            const size_type j = rng.integer<size_type>(0, i);
            std::swap(edges_[i], edges_[j]);
        }
        return *this;
    }

    [[nodiscard]] size_type nodes() const noexcept {
        return nodes_;
    }

    [[nodiscard]] node_type first_node() const noexcept {
        return first_;
    }

    [[nodiscard]] std::vector<edge_type> build() const {
        validate_tree();
        return edges_;
    }

private:
    size_type nodes_{0};
    node_type first_{1};
    std::vector<edge_type> edges_;

    [[nodiscard]] static edge_type Edge(const node_type u, const node_type v, const weight_type w, const bool weighted) noexcept {
        return edge_type{u, v, w, weighted};
    }

    [[nodiscard]] static node_type label_from(const node_type first, const size_type offset) noexcept {
        return static_cast<node_type>(first + static_cast<node_type>(offset));
    }

    [[nodiscard]] bool contains(const node_type node) const noexcept {
        return nodes_ > 0 && node >= first_ && node <= label_from(first_, nodes_ - 1);
    }

    [[nodiscard]] size_type offset_of(const node_type node) const {
        core::require(contains(node), "TreeBuilder node label is outside the builder label range");
        return static_cast<size_type>(node - first_);
    }

    [[nodiscard]] node_type relabel_child(const node_type node, const TreeBuilder& child, const size_type offset) const {
        return label_from(first_, offset + child.offset_of(node));
    }

    void validate_label_capacity() const {
        if (nodes_ == 0) {
            return;
        }
        core::require(nodes_ <= static_cast<size_type>(2147483647), "TreeBuilder node count exceeds int label capacity");
        (void)label_from(first_, nodes_ - 1);
    }

    [[nodiscard]] static std::pair<node_type, node_type> undirected_key(node_type u, node_type v) noexcept {
        if (v < u) {
            std::swap(u, v);
        }
        return std::make_pair(u, v);
    }

    void validate_tree() const {
        validate_label_capacity();
        const size_type expected_edges = nodes_ > 0 ? nodes_ - 1 : 0;
        core::require(edges_.size() == expected_edges, "TreeBuilder result must have exactly n - 1 edges");
        if (nodes_ == 0) {
            return;
        }

        std::vector<size_type> parent(nodes_);
        for (size_type i = 0; i < nodes_; ++i) {
            parent[i] = i;
        }

        std::set<std::pair<node_type, node_type>> seen;
        for (const auto& edge : edges_) {
            core::require(contains(edge.u) && contains(edge.v), "TreeBuilder edge endpoint is outside the label range");
            core::require(edge.u != edge.v, "TreeBuilder edge must not be a self-loop");
            core::require(seen.insert(undirected_key(edge.u, edge.v)).second, "TreeBuilder duplicate edge is not allowed");

            const size_type a = find(parent, offset_of(edge.u));
            const size_type b = find(parent, offset_of(edge.v));
            parent[a] = b;
        }

        const size_type root = find(parent, 0);
        for (size_type i = 1; i < nodes_; ++i) {
            core::require(find(parent, i) == root, "TreeBuilder result must be connected");
        }
    }

    [[nodiscard]] static size_type find(std::vector<size_type>& parent, const size_type value) {
        if (parent[value] == value) {
            return value;
        }
        parent[value] = find(parent, parent[value]);
        return parent[value];
    }
};

inline TreeBuilder Tree::from_edges(const size_type n, const std::vector<edge_type>& edges, const node_type first) {
    return TreeBuilder::from_edges(n, edges, first);
}

} // namespace cp_stress_gen
