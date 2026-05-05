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
};

} // namespace cp_stress_gen::anti

