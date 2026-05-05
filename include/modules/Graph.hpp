#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace cp_stress_gen {

class Graph {
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

    explicit Graph(const size_type nodes) : nodes_(nodes) {}

    Graph(const size_type nodes, const size_type edge_count) : nodes_(nodes), requested_edges_(edge_count), has_edges_(true) {}

    Graph& first_node(const node_type first) noexcept {
        first_ = first;
        return *this;
    }

    Graph& one_based() noexcept {
        return first_node(1);
    }

    Graph& zero_based() noexcept {
        return first_node(0);
    }

    Graph& edges(const size_type count) noexcept {
        requested_edges_ = count;
        has_edges_ = true;
        return *this;
    }

    Graph& directed() noexcept {
        directed_ = true;
        return *this;
    }

    Graph& undirected() noexcept {
        directed_ = false;
        return *this;
    }

    Graph& no_multi_edges() noexcept {
        no_multi_edges_ = true;
        return *this;
    }

    Graph& allow_multi_edges() noexcept {
        no_multi_edges_ = false;
        return *this;
    }

    Graph& weight(const weight_type value) noexcept {
        weighted_ = true;
        weight_left_ = value;
        weight_right_ = value;
        return *this;
    }

    Graph& weighted(const weight_type left, const weight_type right) {
        core::require_range(left, right, "Graph::weighted requires left <= right");
        weighted_ = true;
        weight_left_ = left;
        weight_right_ = right;
        return *this;
    }

    Graph& dag() noexcept {
        mode_ = Mode::Dag;
        directed_ = true;
        return *this;
    }

    Graph& layered_dag(const size_type layers) noexcept {
        mode_ = Mode::LayeredDag;
        layers_ = layers;
        directed_ = true;
        return *this;
    }

    Graph& dense() noexcept {
        mode_ = Mode::Dense;
        no_multi_edges_ = true;
        return *this;
    }

    Graph& sparse_connected() noexcept {
        mode_ = Mode::SparseConnected;
        no_multi_edges_ = true;
        return *this;
    }

    Graph& complete() noexcept {
        mode_ = Mode::Complete;
        no_multi_edges_ = true;
        return *this;
    }

    Graph& cycle() noexcept {
        mode_ = Mode::Cycle;
        no_multi_edges_ = true;
        return *this;
    }

    Graph& path() noexcept {
        mode_ = Mode::Path;
        no_multi_edges_ = true;
        return *this;
    }

    Graph& forest(const size_type components) noexcept {
        mode_ = Mode::Forest;
        components_ = components;
        no_multi_edges_ = true;
        directed_ = false;
        return *this;
    }

    Graph& connected_components(const size_type components) noexcept {
        mode_ = Mode::ConnectedComponents;
        components_ = components;
        no_multi_edges_ = true;
        directed_ = false;
        return *this;
    }

    Graph& bipartite(const size_type left_size) noexcept {
        mode_ = Mode::Bipartite;
        left_part_ = left_size;
        return *this;
    }

    Graph& wheel() noexcept {
        mode_ = Mode::Wheel;
        directed_ = false;
        no_multi_edges_ = true;
        return *this;
    }

    Graph& grid(const size_type rows, const size_type cols) noexcept {
        mode_ = Mode::Grid;
        rows_ = rows;
        cols_ = cols;
        directed_ = false;
        no_multi_edges_ = true;
        return *this;
    }

    Graph& complete_bipartite(const size_type left_size, const size_type right_size) noexcept {
        mode_ = Mode::CompleteBipartite;
        left_part_ = left_size;
        right_part_ = right_size;
        directed_ = false;
        no_multi_edges_ = true;
        return *this;
    }

    Graph& tournament() noexcept {
        mode_ = Mode::Tournament;
        directed_ = true;
        no_multi_edges_ = true;
        return *this;
    }

    Graph& shuffle() noexcept {
        shuffle_ = true;
        return *this;
    }

    [[nodiscard]] size_type nodes() const noexcept {
        return nodes_;
    }

    [[nodiscard]] std::vector<edge_type> build(core::Random& rng) const {
        validate_labels();

        std::vector<edge_type> result;
        if (mode_ == Mode::Dag) {
            generate_dag(result, rng);
        } else if (mode_ == Mode::LayeredDag) {
            generate_layered_dag(result, rng);
        } else if (mode_ == Mode::Dense) {
            generate_dense(result, rng);
        } else if (mode_ == Mode::SparseConnected) {
            generate_sparse_connected(result, rng);
        } else if (mode_ == Mode::Complete) {
            generate_complete(result, rng);
        } else if (mode_ == Mode::Cycle) {
            generate_cycle(result, rng);
        } else if (mode_ == Mode::Path) {
            generate_path(result, rng);
        } else if (mode_ == Mode::Forest) {
            generate_forest(result, rng);
        } else if (mode_ == Mode::ConnectedComponents) {
            generate_connected_components(result, rng);
        } else if (mode_ == Mode::Bipartite) {
            generate_bipartite(result, rng);
        } else if (mode_ == Mode::Wheel) {
            generate_wheel(result, rng);
        } else if (mode_ == Mode::Grid) {
            generate_grid(result, rng);
        } else if (mode_ == Mode::CompleteBipartite) {
            generate_complete_bipartite(result, rng);
        } else if (mode_ == Mode::Tournament) {
            generate_tournament(result, rng);
        } else {
            generate_general(result, rng);
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
        General,
        Dag,
        LayeredDag,
        Dense,
        SparseConnected,
        Complete,
        Cycle,
        Path,
        Forest,
        ConnectedComponents,
        Bipartite,
        Wheel,
        Grid,
        CompleteBipartite,
        Tournament
    };

    size_type nodes_;
    size_type requested_edges_{0};
    bool has_edges_{false};
    node_type first_{1};
    bool directed_{false};
    bool no_multi_edges_{false};
    bool weighted_{false};
    weight_type weight_left_{1};
    weight_type weight_right_{1};
    size_type layers_{0};
    size_type left_part_{0};
    size_type right_part_{0};
    size_type rows_{0};
    size_type cols_{0};
    size_type components_{0};
    bool shuffle_{false};
    Mode mode_{Mode::General};

    void validate_labels() const {
        if (nodes_ == 0) {
            return;
        }
        core::require(nodes_ <= static_cast<size_type>(2147483647), "Graph node count exceeds int label capacity");
        (void)label(nodes_ - 1);
    }

    [[nodiscard]] node_type label(const size_type offset) const noexcept {
        return static_cast<node_type>(first_ + static_cast<node_type>(offset));
    }

    [[nodiscard]] weight_type next_weight(core::Random& rng) const {
        return weighted_ ? rng.integer<weight_type>(weight_left_, weight_right_) : weight_type{1};
    }

    [[nodiscard]] edge_type make_edge(const size_type u, const size_type v, core::Random& rng) const {
        return Edge{label(u), label(v), next_weight(rng), weighted_};
    }

    [[nodiscard]] std::uint64_t max_directed_edges() const noexcept {
        const std::uint64_t n = static_cast<std::uint64_t>(nodes_);
        return n * (n - 1);
    }

    [[nodiscard]] std::uint64_t max_undirected_edges() const noexcept {
        const std::uint64_t n = static_cast<std::uint64_t>(nodes_);
        return n * (n - 1) / 2;
    }

    [[nodiscard]] std::uint64_t max_current_edges() const noexcept {
        return directed_ ? max_directed_edges() : max_undirected_edges();
    }

    [[nodiscard]] size_type requested_or_default(const std::uint64_t maximum, const size_type fallback, const char* name) const {
        const std::uint64_t count = has_edges_ ? static_cast<std::uint64_t>(requested_edges_) : static_cast<std::uint64_t>(fallback);
        core::require(count <= maximum, std::string(name) + " requested edge count is impossible");
        return static_cast<size_type>(count);
    }

    [[nodiscard]] size_type requested_or_default_unbounded(const size_type fallback) const noexcept {
        return has_edges_ ? requested_edges_ : fallback;
    }

    void add_unique_undirected(std::vector<edge_type>& result, const size_type count, core::Random& rng) const {
        result.reserve(count);
        for (size_type u = 0; u + 1 < nodes_ && result.size() < count; ++u) {
            for (size_type v = u + 1; v < nodes_ && result.size() < count; ++v) {
                result.push_back(make_edge(u, v, rng));
            }
        }
    }

    void add_unique_directed(std::vector<edge_type>& result, const size_type count, core::Random& rng) const {
        result.reserve(count);
        for (size_type u = 0; u < nodes_ && result.size() < count; ++u) {
            for (size_type v = 0; v < nodes_ && result.size() < count; ++v) {
                if (u != v) {
                    result.push_back(make_edge(u, v, rng));
                }
            }
        }
    }

    void generate_general(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(nodes_ >= 2 || !has_edges_ || requested_edges_ == 0, "Graph needs at least 2 nodes for edges");
        if (no_multi_edges_) {
            const size_type count = requested_or_default(max_current_edges(), 0, "Graph");
            if (directed_) {
                add_unique_directed(result, count, rng);
            } else {
                add_unique_undirected(result, count, rng);
            }
            return;
        }

        const size_type count = requested_or_default_unbounded(0);
        result.reserve(count);
        for (size_type i = 0; i < count; ++i) {
            size_type u = rng.integer<size_type>(0, nodes_ - 1);
            size_type v = rng.integer<size_type>(0, nodes_ - 2);
            if (v >= u) {
                ++v;
            }
            if (!directed_ && v < u) {
                std::swap(u, v);
            }
            result.push_back(make_edge(u, v, rng));
        }
    }

    void generate_dag(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(nodes_ >= 2 || !has_edges_ || requested_edges_ == 0, "Graph::dag needs at least 2 nodes for edges");
        const size_type fallback = nodes_ > 0 ? nodes_ - 1 : 0;
        const size_type count = no_multi_edges_
            ? requested_or_default(max_undirected_edges(), fallback, "Graph::dag")
            : requested_or_default_unbounded(fallback);

        result.reserve(count);
        if (no_multi_edges_) {
            for (size_type u = 0; u + 1 < nodes_ && result.size() < count; ++u) {
                for (size_type v = u + 1; v < nodes_ && result.size() < count; ++v) {
                    result.push_back(make_edge(u, v, rng));
                }
            }
            return;
        }

        for (size_type i = 0; i < count; ++i) {
            const size_type u = rng.integer<size_type>(0, nodes_ - 2);
            const size_type v = rng.integer<size_type>(u + 1, nodes_ - 1);
            result.push_back(make_edge(u, v, rng));
        }
    }

    void generate_layered_dag(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(nodes_ >= 2 || !has_edges_ || requested_edges_ == 0, "Graph::layered_dag needs at least 2 nodes for edges");
        core::require(layers_ >= 2 && layers_ <= nodes_, "Graph::layered_dag layers must be in [2, n]");

        std::vector<size_type> layer_of(nodes_);
        for (size_type i = 0; i < nodes_; ++i) {
            layer_of[i] = i * layers_ / nodes_;
        }

        std::uint64_t maximum = 0;
        for (size_type u = 0; u < nodes_; ++u) {
            for (size_type v = 0; v < nodes_; ++v) {
                if (layer_of[u] < layer_of[v]) {
                    ++maximum;
                }
            }
        }

        const size_type fallback = static_cast<size_type>(maximum);
        const size_type count = no_multi_edges_
            ? requested_or_default(maximum, fallback, "Graph::layered_dag")
            : requested_or_default_unbounded(fallback);
        result.reserve(count);
        if (no_multi_edges_) {
            for (size_type u = 0; u < nodes_ && result.size() < count; ++u) {
                for (size_type v = 0; v < nodes_ && result.size() < count; ++v) {
                    if (layer_of[u] < layer_of[v]) {
                        result.push_back(make_edge(u, v, rng));
                    }
                }
            }
            return;
        }

        std::vector<std::pair<size_type, size_type>> pairs;
        pairs.reserve(static_cast<size_type>(maximum));
        for (size_type u = 0; u < nodes_; ++u) {
            for (size_type v = 0; v < nodes_; ++v) {
                if (layer_of[u] < layer_of[v]) {
                    pairs.push_back(std::make_pair(u, v));
                }
            }
        }
        core::require(!pairs.empty() || count == 0, "Graph::layered_dag has no valid layer-crossing edges");
        for (size_type i = 0; i < count; ++i) {
            const size_type index = rng.integer<size_type>(0, pairs.size() - 1);
            result.push_back(make_edge(pairs[index].first, pairs[index].second, rng));
        }
    }

    void generate_dense(std::vector<edge_type>& result, core::Random& rng) const {
        const std::uint64_t maximum = max_current_edges();
        const size_type fallback = static_cast<size_type>((maximum * 3 + 3) / 4);
        const size_type count = requested_or_default(maximum, fallback, "Graph::dense");
        if (directed_) {
            add_unique_directed(result, count, rng);
        } else {
            add_unique_undirected(result, count, rng);
        }
    }

    void generate_sparse_connected(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(!directed_, "Graph::sparse_connected currently supports undirected graphs only");
        core::require(nodes_ >= 1, "Graph::sparse_connected needs at least one node");
        const size_type minimum = nodes_ > 0 ? nodes_ - 1 : 0;
        const size_type count = requested_or_default(max_undirected_edges(), minimum, "Graph::sparse_connected");
        core::require(count >= minimum, "Graph::sparse_connected needs at least n - 1 edges");

        result.reserve(count);
        for (size_type i = 0; i + 1 < nodes_; ++i) {
            result.push_back(make_edge(i, i + 1, rng));
        }
        for (size_type u = 0; u + 1 < nodes_ && result.size() < count; ++u) {
            for (size_type v = u + 2; v < nodes_ && result.size() < count; ++v) {
                result.push_back(make_edge(u, v, rng));
            }
        }
    }

    void generate_complete(std::vector<edge_type>& result, core::Random& rng) const {
        const size_type count = static_cast<size_type>(max_current_edges());
        if (has_edges_) {
            core::require(requested_edges_ == count, "Graph::complete edge count must equal the complete graph size");
        }
        if (directed_) {
            add_unique_directed(result, count, rng);
        } else {
            add_unique_undirected(result, count, rng);
        }
    }

    void generate_cycle(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(nodes_ >= 3, "Graph::cycle needs at least 3 nodes");
        if (has_edges_) {
            core::require(requested_edges_ == nodes_, "Graph::cycle edge count must equal n");
        }
        result.reserve(nodes_);
        for (size_type i = 0; i + 1 < nodes_; ++i) {
            result.push_back(make_edge(i, i + 1, rng));
        }
        result.push_back(make_edge(nodes_ - 1, 0, rng));
    }

    void generate_path(std::vector<edge_type>& result, core::Random& rng) const {
        const size_type count = nodes_ > 0 ? nodes_ - 1 : 0;
        if (has_edges_) {
            core::require(requested_edges_ == count, "Graph::path edge count must equal n - 1");
        }
        result.reserve(count);
        for (size_type i = 0; i + 1 < nodes_; ++i) {
            result.push_back(make_edge(i, i + 1, rng));
        }
    }

    void validate_components(const char* name) const {
        if (nodes_ == 0) {
            core::require(components_ == 0, std::string(name) + " requires 0 components when n = 0");
            return;
        }
        core::require(components_ >= 1 && components_ <= nodes_, std::string(name) + " components must be in [1, n]");
    }

    void generate_forest(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(!directed_, "Graph::forest supports undirected graphs only");
        validate_components("Graph::forest");
        const size_type count = nodes_ >= components_ ? nodes_ - components_ : 0;
        if (has_edges_) {
            core::require(requested_edges_ == count, "Graph::forest edge count must equal n - components");
        }
        result.reserve(count);
        for (size_type c = 0; c < components_; ++c) {
            const size_type begin = c * nodes_ / components_;
            const size_type end = (c + 1) * nodes_ / components_;
            for (size_type i = begin; i + 1 < end; ++i) {
                result.push_back(make_edge(i, i + 1, rng));
            }
        }
    }

    void generate_connected_components(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(!directed_, "Graph::connected_components supports undirected graphs only");
        validate_components("Graph::connected_components");

        std::uint64_t maximum = 0;
        for (size_type c = 0; c < components_; ++c) {
            const size_type begin = c * nodes_ / components_;
            const size_type end = (c + 1) * nodes_ / components_;
            const std::uint64_t size = static_cast<std::uint64_t>(end - begin);
            maximum += size * (size - 1) / 2;
        }

        const size_type minimum = nodes_ >= components_ ? nodes_ - components_ : 0;
        const size_type count = requested_or_default(maximum, minimum, "Graph::connected_components");
        core::require(count >= minimum, "Graph::connected_components edge count must be at least n - components");

        result.reserve(count);
        for (size_type c = 0; c < components_; ++c) {
            const size_type begin = c * nodes_ / components_;
            const size_type end = (c + 1) * nodes_ / components_;
            for (size_type i = begin; i + 1 < end; ++i) {
                result.push_back(make_edge(i, i + 1, rng));
            }
        }

        for (size_type c = 0; c < components_ && result.size() < count; ++c) {
            const size_type begin = c * nodes_ / components_;
            const size_type end = (c + 1) * nodes_ / components_;
            for (size_type u = begin; u + 1 < end && result.size() < count; ++u) {
                for (size_type v = u + 2; v < end && result.size() < count; ++v) {
                    result.push_back(make_edge(u, v, rng));
                }
            }
        }
    }

    void generate_bipartite(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(left_part_ > 0 && left_part_ < nodes_, "Graph::bipartite left size must be in [1, n - 1]");
        const size_type right_part = nodes_ - left_part_;
        const std::uint64_t maximum = static_cast<std::uint64_t>(left_part_) * right_part;
        const size_type fallback = static_cast<size_type>(maximum);
        const size_type count = no_multi_edges_
            ? requested_or_default(maximum, fallback, "Graph::bipartite")
            : requested_or_default_unbounded(fallback);

        result.reserve(count);
        if (no_multi_edges_) {
            for (size_type u = 0; u < left_part_ && result.size() < count; ++u) {
                for (size_type v = left_part_; v < nodes_ && result.size() < count; ++v) {
                    result.push_back(make_edge(u, v, rng));
                }
            }
            return;
        }

        for (size_type i = 0; i < count; ++i) {
            const size_type u = rng.integer<size_type>(0, left_part_ - 1);
            const size_type v = rng.integer<size_type>(left_part_, nodes_ - 1);
            result.push_back(make_edge(u, v, rng));
        }
    }

    void require_exact_edges(const size_type expected, const char* name) const {
        if (has_edges_) {
            core::require(requested_edges_ == expected, std::string(name) + " edge count must match the generated structure");
        }
    }

    void generate_wheel(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(!directed_, "Graph::wheel supports undirected graphs only");
        core::require(nodes_ >= 4, "Graph::wheel needs at least 4 nodes");
        const size_type expected = 2 * (nodes_ - 1);
        require_exact_edges(expected, "Graph::wheel");

        result.reserve(expected);
        for (size_type i = 1; i + 1 < nodes_; ++i) {
            result.push_back(make_edge(i, i + 1, rng));
        }
        result.push_back(make_edge(nodes_ - 1, 1, rng));
        for (size_type i = 1; i < nodes_; ++i) {
            result.push_back(make_edge(0, i, rng));
        }
    }

    void generate_grid(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(!directed_, "Graph::grid supports undirected graphs only");
        core::require(rows_ > 0 && cols_ > 0, "Graph::grid rows and cols must be positive");
        core::require(rows_ <= nodes_ / cols_ && rows_ * cols_ == nodes_, "Graph::grid rows * cols must equal n");
        const size_type expected = (rows_ - 1) * cols_ + rows_ * (cols_ - 1);
        require_exact_edges(expected, "Graph::grid");

        result.reserve(expected);
        for (size_type r = 0; r < rows_; ++r) {
            for (size_type c = 0; c < cols_; ++c) {
                const size_type id = r * cols_ + c;
                if (r + 1 < rows_) {
                    result.push_back(make_edge(id, id + cols_, rng));
                }
                if (c + 1 < cols_) {
                    result.push_back(make_edge(id, id + 1, rng));
                }
            }
        }
    }

    void generate_complete_bipartite(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(!directed_, "Graph::complete_bipartite supports undirected graphs only");
        core::require(left_part_ > 0 && right_part_ > 0, "Graph::complete_bipartite parts must be positive");
        core::require(left_part_ + right_part_ == nodes_, "Graph::complete_bipartite left + right must equal n");
        const size_type expected = left_part_ * right_part_;
        require_exact_edges(expected, "Graph::complete_bipartite");

        result.reserve(expected);
        for (size_type u = 0; u < left_part_; ++u) {
            for (size_type v = left_part_; v < nodes_; ++v) {
                result.push_back(make_edge(u, v, rng));
            }
        }
    }

    void generate_tournament(std::vector<edge_type>& result, core::Random& rng) const {
        core::require(directed_, "Graph::tournament supports directed graphs only");
        const std::uint64_t n = static_cast<std::uint64_t>(nodes_);
        const size_type expected = static_cast<size_type>(n * (n > 0 ? n - 1 : 0) / 2);
        require_exact_edges(expected, "Graph::tournament");

        result.reserve(expected);
        for (size_type u = 0; u + 1 < nodes_; ++u) {
            for (size_type v = u + 1; v < nodes_; ++v) {
                if (rng.boolean()) {
                    result.push_back(make_edge(u, v, rng));
                } else {
                    result.push_back(make_edge(v, u, rng));
                }
            }
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
