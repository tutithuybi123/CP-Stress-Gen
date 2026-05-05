#pragma once

#include "../core/Random.hpp"
#include "../modules/Graph.hpp"

#include <cstddef>
#include <vector>

namespace cp_stress_gen::anti {

class GraphKiller {
public:
    using size_type = std::size_t;

    [[nodiscard]] static std::vector<Graph::Edge> dense(const size_type n, core::Random& rng) {
        return Graph(n).dense().build(rng);
    }

    [[nodiscard]] static std::vector<Graph::Edge> long_dag_chain(const size_type n) {
        std::vector<Graph::Edge> result;
        if (n < 2) {
            return result;
        }
        result.reserve(n - 1);
        for (size_type i = 1; i < n; ++i) {
            result.push_back(Graph::Edge{static_cast<int>(i), static_cast<int>(i + 1), 1, false});
        }
        return result;
    }

    [[nodiscard]] static std::vector<Graph::Edge> sparse_connected(const size_type n, core::Random& rng) {
        return Graph(n).sparse_connected().build(rng);
    }

    [[nodiscard]] static std::vector<Graph::Edge> dense_dag(const size_type n) {
        std::vector<Graph::Edge> result;
        result.reserve(n * (n > 0 ? n - 1 : 0) / 2);
        for (size_type u = 1; u <= n; ++u) {
            for (size_type v = u + 1; v <= n; ++v) {
                result.push_back(Graph::Edge{static_cast<int>(u), static_cast<int>(v), 1, false});
            }
        }
        return result;
    }
};

} // namespace cp_stress_gen::anti
