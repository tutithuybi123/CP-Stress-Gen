#pragma once

#include "../core/Random.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <utility>
#include <vector>

namespace cp_stress_gen {

class Tree {
public:
    using node_type = int;
    using edge_type = std::pair<node_type, node_type>;
    using size_type = std::size_t;

    explicit Tree(const size_type nodes)
        : nodes_(nodes), random_(core::Random::from_time()) {}

    Tree(const size_type nodes, const std::uint64_t seed)
        : nodes_(nodes), random_(seed) {}

    Tree(const size_type nodes, core::Random random) noexcept
        : nodes_(nodes), random_(random) {}

    [[nodiscard]] static Tree seeded(const size_type nodes, const std::uint64_t seed) {
        return Tree(nodes, seed);
    }

    Tree& seed(const std::uint64_t seed_value) noexcept {
        random_.seed(seed_value);
        return *this;
    }

    Tree& bamboo(const node_type first = 1) {
        edges_.clear();
        if (nodes_ < 2) {
            return *this;
        }

        edges_.reserve(nodes_ - 1);
        for (const size_type i : std::views::iota(size_type{0}, nodes_ - 1)) {
            const auto u = static_cast<node_type>(first + static_cast<node_type>(i));
            edges_.emplace_back(u, static_cast<node_type>(u + 1));
        }
        return *this;
    }

    Tree& star(const node_type center = 1, const node_type first = 1) {
        edges_.clear();
        if (nodes_ < 2) {
            return *this;
        }

        const node_type root = normalized_center(center, first);
        edges_.reserve(nodes_ - 1);
        for (const size_type i : std::views::iota(size_type{0}, nodes_)) {
            const auto v = static_cast<node_type>(first + static_cast<node_type>(i));
            if (v != root) {
                edges_.emplace_back(root, v);
            }
        }
        return *this;
    }

    Tree& random(const node_type first = 1) {
        edges_.clear();
        if (nodes_ < 2) {
            return *this;
        }

        edges_.reserve(nodes_ - 1);
        for (const size_type i : std::views::iota(size_type{1}, nodes_)) {
            const auto v = static_cast<node_type>(first + static_cast<node_type>(i));
            const auto parent = random_.integer<node_type>(
                first,
                static_cast<node_type>(first + static_cast<node_type>(i) - 1)
            );
            edges_.emplace_back(parent, v);
        }
        return *this;
    }

    Tree& shuffle() noexcept {
        if (edges_.size() < 2) {
            return *this;
        }

        for (size_type i = edges_.size() - 1; i > 0; --i) {
            const size_type j = random_.integer<size_type>(0, i);
            std::swap(edges_[i], edges_[j]);
        }
        return *this;
    }

    [[nodiscard]] size_type nodes() const noexcept {
        return nodes_;
    }

    [[nodiscard]] size_type edges_count() const noexcept {
        return edges_.size();
    }

    [[nodiscard]] const std::vector<edge_type>& view() const noexcept {
        return edges_;
    }

    [[nodiscard]] std::vector<edge_type> build() const& {
        return edges_;
    }

    [[nodiscard]] std::vector<edge_type> build() && noexcept {
        return std::move(edges_);
    }

private:
    size_type nodes_;
    std::vector<edge_type> edges_;
    core::Random random_;

    [[nodiscard]] node_type normalized_center(const node_type center, const node_type first) const noexcept {
        const auto last = static_cast<node_type>(first + static_cast<node_type>(nodes_ - 1));
        if (center < first || center > last) {
            return first;
        }
        return center;
    }
};

} // namespace cp_stress_gen

