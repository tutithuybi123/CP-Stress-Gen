#pragma once

#include "../core/Random.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <utility>
#include <vector>

namespace cp_stress_gen {

class Graph {
public:
    using node_type = int;
    using edge_type = std::pair<node_type, node_type>;
    using size_type = std::size_t;

    explicit Graph(const size_type nodes)
        : nodes_(nodes), random_(core::Random::from_time()) {}

    Graph(const size_type nodes, const std::uint64_t seed)
        : nodes_(nodes), random_(seed) {}

    Graph(const size_type nodes, core::Random random) noexcept
        : nodes_(nodes), random_(random) {}

    [[nodiscard]] static Graph seeded(const size_type nodes, const std::uint64_t seed) {
        return Graph(nodes, seed);
    }

    Graph& seed(const std::uint64_t seed_value) noexcept {
        random_.seed(seed_value);
        return *this;
    }

    Graph& bamboo(const node_type first = 1) {
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

    Graph& star(const node_type center = 1, const node_type first = 1) {
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

    Graph& dag(const size_type edge_count, const node_type first = 1) {
        edges_.clear();
        if (nodes_ < 2 || edge_count == 0) {
            return *this;
        }

        const size_type limit = clamped_dag_edges(edge_count);
        edges_.reserve(limit);

        const auto max_source = static_cast<node_type>(nodes_ - 2);
        for ([[maybe_unused]] const size_type _ : std::views::iota(size_type{0}, limit)) {
            const auto u_offset = random_.integer<node_type>(0, max_source);
            const auto v_offset = random_.integer<node_type>(
                static_cast<node_type>(u_offset + 1),
                static_cast<node_type>(nodes_ - 1)
            );
            edges_.emplace_back(
                static_cast<node_type>(first + u_offset),
                static_cast<node_type>(first + v_offset)
            );
        }
        return *this;
    }

    Graph& shuffle() noexcept {
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

    [[nodiscard]] size_type clamped_dag_edges(const size_type requested) const noexcept {
        const auto n = static_cast<std::uint64_t>(nodes_);
        const auto maximum = static_cast<std::uint64_t>(n * (n - 1) / 2);
        const auto limited = std::min<std::uint64_t>(requested, maximum);
        return static_cast<size_type>(limited);
    }

    [[nodiscard]] node_type normalized_center(const node_type center, const node_type first) const noexcept {
        const auto last = static_cast<node_type>(first + static_cast<node_type>(nodes_ - 1));
        if (center < first || center > last) {
            return first;
        }
        return center;
    }
};

} // namespace cp_stress_gen

