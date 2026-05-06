#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"
#include "../modules/Array.hpp"
#include "../modules/Graph.hpp"
#include "../modules/Tree.hpp"
#include "StringKiller.hpp"
#include "TreeKiller.hpp"

#include <algorithm>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace cp_stress_gen::anti {

struct DsuQuery {
    enum Type {
        Unite,
        Same
    };

    Type type;
    int u;
    int v;
};

struct Interval {
    int l;
    int r;
    int weight;
};

struct KnapsackItem {
    int weight;
    int value;
};

class DfsBfsKiller {
public:
    using size_type = std::size_t;

    [[nodiscard]] static std::vector<Tree::Edge> deep_chain(const size_type n) {
        return TreeKiller::chain(n);
    }

    [[nodiscard]] static std::vector<Graph::Edge> many_components(const size_type n, const size_type components) {
        core::Random rng(1);
        return Graph(n).forest(components).build(rng);
    }

    [[nodiscard]] static std::vector<Tree::Edge> huge_branching(const size_type n) {
        return TreeKiller::star(n);
    }

    [[nodiscard]] static std::vector<Tree::Edge> broom_tree(const size_type n, const size_type chain_len) {
        return TreeKiller::broom(n, chain_len);
    }

    [[nodiscard]] static std::vector<Graph::Edge> grid_maze_like(const size_type rows, const size_type cols) {
        core::Random rng(1);
        return Graph(rows * cols).grid(rows, cols).build(rng);
    }
};

class DijkstraKiller {
public:
    using size_type = std::size_t;

    [[nodiscard]] static std::vector<Graph::Edge> many_equal_distances(const size_type n, const size_type m) {
        core::Random rng(2);
        return Graph(n).edges(m).no_multi_edges().weight(1).build(rng);
    }

    [[nodiscard]] static std::vector<Graph::Edge> zero_weight_edges(const size_type n, const size_type m) {
        core::Random rng(3);
        return Graph(n).edges(m).no_multi_edges().weight(0).build(rng);
    }

    [[nodiscard]] static std::vector<Graph::Edge> dense_weighted_trap(const size_type n) {
        core::Random rng(4);
        return Graph(n).dense().weighted(1, 2).build(rng);
    }

    [[nodiscard]] static std::vector<Graph::Edge> layered_equal_shortest_paths(const size_type layers, const size_type width) {
        core::require(layers >= 2, "DijkstraKiller::layered_equal_shortest_paths needs at least two layers");
        core::require_positive(width, "DijkstraKiller::layered_equal_shortest_paths width must be positive");

        std::vector<Graph::Edge> result;
        result.reserve((layers - 1) * width * width);
        for (size_type layer = 0; layer + 1 < layers; ++layer) {
            for (size_type from = 0; from < width; ++from) {
                for (size_type to = 0; to < width; ++to) {
                    const int u = static_cast<int>(layer * width + from + 1);
                    const int v = static_cast<int>((layer + 1) * width + to + 1);
                    result.push_back(Graph::Edge{u, v, 1, true});
                }
            }
        }
        return result;
    }
};

class DsuKiller {
public:
    using size_type = std::size_t;

    [[nodiscard]] static std::vector<Graph::Edge> redundant_edges(const size_type n, const size_type extra) {
        core::require(n > 0 || extra == 0, "DsuKiller::redundant_edges needs nodes when extra > 0");
        std::vector<Graph::Edge> result;
        result.reserve((n > 0 ? n - 1 : 0) + extra);
        for (size_type i = 1; i < n; ++i) {
            result.push_back(Graph::Edge{static_cast<int>(i), static_cast<int>(i + 1), 1, false});
        }
        for (size_type i = 0; i < extra; ++i) {
            result.push_back(Graph::Edge{1, static_cast<int>(n), 1, false});
        }
        return result;
    }

    [[nodiscard]] static std::vector<Graph::Edge> delayed_connectivity(const size_type n) {
        std::vector<Graph::Edge> result;
        result.reserve(n > 0 ? n - 1 : 0);
        for (size_type i = n; i > 1; --i) {
            result.push_back(Graph::Edge{static_cast<int>(i - 1), static_cast<int>(i), 1, false});
        }
        return result;
    }

    [[nodiscard]] static std::vector<DsuQuery> reverse_union_sequence(const size_type n) {
        std::vector<DsuQuery> result;
        result.reserve(n > 0 ? n - 1 : 0);
        for (size_type i = n; i > 1; --i) {
            result.push_back(DsuQuery{DsuQuery::Unite, static_cast<int>(i - 1), static_cast<int>(i)});
        }
        return result;
    }

    [[nodiscard]] static std::vector<DsuQuery> many_queries_same_component(const size_type n, const size_type q) {
        core::require(n > 0 || q == 0, "DsuKiller::many_queries_same_component needs nodes when q > 0");
        std::vector<DsuQuery> result = reverse_union_sequence(n);
        result.reserve(result.size() + q);
        for (size_type i = 0; i < q; ++i) {
            result.push_back(DsuQuery{DsuQuery::Same, 1, static_cast<int>(n)});
        }
        return result;
    }
};

class DpKiller {
public:
    using size_type = std::size_t;
    using value_type = long long;

    [[nodiscard]] static std::vector<value_type> all_equal_array(const size_type n, const value_type value) {
        return std::vector<value_type>(n, value);
    }

    [[nodiscard]] static std::vector<value_type> monotonic_array(const size_type n) {
        return Array(n).iota(1, 1).build();
    }

    [[nodiscard]] static std::vector<value_type> alternating_array(const size_type n, const value_type low, const value_type high) {
        std::vector<value_type> result;
        result.reserve(n);
        for (size_type i = 0; i < n; ++i) {
            result.push_back(i % 2 == 0 ? low : high);
        }
        return result;
    }

    [[nodiscard]] static std::vector<size_type> boundary_sizes(const size_type n) {
        std::vector<size_type> result{0, 1, n};
        if (n > 1) {
            result.push_back(n - 1);
        }
        std::sort(result.begin(), result.end());
        result.erase(std::unique(result.begin(), result.end()), result.end());
        return result;
    }

    [[nodiscard]] static std::vector<KnapsackItem> knapsack_tight_capacity(const size_type items, const int capacity) {
        core::require_positive(items, "DpKiller::knapsack_tight_capacity item count must be positive");
        core::require(capacity > 0, "DpKiller::knapsack_tight_capacity capacity must be positive");
        std::vector<KnapsackItem> result;
        result.reserve(items);
        for (size_type i = 0; i < items; ++i) {
            const int weight = std::max(1, capacity / 2 + static_cast<int>(i % 3) - 1);
            result.push_back(KnapsackItem{weight, weight});
        }
        return result;
    }
};

class GreedyKiller {
public:
    using size_type = std::size_t;
    using value_type = long long;

    [[nodiscard]] static std::vector<value_type> local_optimum_trap_array(const size_type n) {
        std::vector<value_type> result;
        result.reserve(n);
        for (size_type i = 0; i < n; i += 2) {
            if (i + 1 < n) {
                result.push_back(static_cast<value_type>(i + 2));
                result.push_back(static_cast<value_type>(i + 1));
            } else {
                result.push_back(static_cast<value_type>(i + 1));
            }
        }
        return result;
    }

    [[nodiscard]] static std::vector<Interval> interval_scheduling_trap(const size_type n) {
        std::vector<Interval> result;
        if (n == 0) {
            return result;
        }
        result.reserve(n);
        result.push_back(Interval{1, static_cast<int>(n + 1), static_cast<int>(n)});
        for (size_type i = 1; i < n; ++i) {
            result.push_back(Interval{static_cast<int>(i), static_cast<int>(i + 1), 1});
        }
        return result;
    }

    [[nodiscard]] static std::vector<int> coin_change_greedy_trap() {
        return std::vector<int>{1, 3, 4};
    }
};

class BinarySearchKiller {
public:
    using size_type = std::size_t;

    [[nodiscard]] static std::vector<long long> off_by_one_boundaries(const size_type n) {
        if (n == 0) {
            return std::vector<long long>{0};
        }
        return std::vector<long long>{0, static_cast<long long>(n - 1), static_cast<long long>(n)};
    }

    [[nodiscard]] static std::vector<long long> lower_upper_bound_duplicates(const size_type n) {
        std::vector<long long> result;
        result.reserve(n);
        for (size_type i = 0; i < n; ++i) {
            result.push_back(i < n / 2 ? 1 : 2);
        }
        return result;
    }

    [[nodiscard]] static std::vector<int> all_false_then_true(const size_type n, const size_type pivot) {
        core::require(pivot <= n, "BinarySearchKiller::all_false_then_true pivot must be <= n");
        std::vector<int> result(n, 0);
        for (size_type i = pivot; i < n; ++i) {
            result[i] = 1;
        }
        return result;
    }

    [[nodiscard]] static std::vector<int> all_true_then_false(const size_type n, const size_type pivot) {
        core::require(pivot <= n, "BinarySearchKiller::all_true_then_false pivot must be <= n");
        std::vector<int> result(n, 0);
        for (size_type i = 0; i < pivot; ++i) {
            result[i] = 1;
        }
        return result;
    }
};

class HashKiller {
public:
    using size_type = std::size_t;

    [[nodiscard]] static std::string repeated_base_sensitive_shapes(const size_type n) {
        if (n == 0) {
            return {};
        }
        std::string result;
        result.reserve(n);
        for (size_type i = 0; i < n; ++i) {
            result.push_back(i % 2 == 0 ? 'a' : 'z');
        }
        return result;
    }

    [[nodiscard]] static std::pair<std::string, std::string> collision_like_patterns(const size_type n) {
        core::require(n > 0, "HashKiller::collision_like_patterns length must be positive");
        std::string first(n, 'a');
        std::string second(n, 'a');
        first[n - 1] = 'b';
        second[0] = 'b';
        return std::make_pair(first, second);
    }
};

class StringPatternKiller {
public:
    using size_type = std::size_t;

    [[nodiscard]] static std::string kmp_worst_prefix(const size_type n) {
        return StringKiller::kmp_prefix_pattern(n);
    }

    [[nodiscard]] static std::string z_repeated_blocks(const size_type n) {
        return std::string(n, 'a');
    }

    [[nodiscard]] static std::string border_heavy(const size_type n) {
        return std::string(n, 'a');
    }

    [[nodiscard]] static std::string periodic_with_break(const size_type n) {
        if (n == 0) {
            return {};
        }
        std::string result;
        result.reserve(n);
        for (size_type i = 0; i < n; ++i) {
            result.push_back(i % 2 == 0 ? 'a' : 'b');
        }
        result[n - 1] = 'c';
        return result;
    }
};

} // namespace cp_stress_gen::anti
