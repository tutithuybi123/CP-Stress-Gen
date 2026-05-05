#pragma once

#include "../core/Random.hpp"
#include "../modules/Tree.hpp"

#include <cstddef>
#include <vector>

namespace cp_stress_gen::anti {

class TreeKiller {
public:
    using size_type = std::size_t;

    [[nodiscard]] static std::vector<Tree::Edge> chain(const size_type n) {
        core::Random rng(1);
        return Tree(n).bamboo().build(rng);
    }

    [[nodiscard]] static std::vector<Tree::Edge> star(const size_type n) {
        core::Random rng(1);
        return Tree(n).star().build(rng);
    }

    [[nodiscard]] static std::vector<Tree::Edge> chain_with_branches(const size_type n, const size_type branches, core::Random& rng) {
        return Tree(n).deep_recursion(branches).build(rng);
    }

    [[nodiscard]] static std::vector<Tree::Edge> broom(const size_type n, const size_type handle_length) {
        if (n == 0) {
            return {};
        }
        core::require(handle_length > 0 && handle_length <= n, "TreeKiller::broom handle length must be in [1, n]");

        std::vector<Tree::Edge> result;
        result.reserve(n - 1);
        for (size_type i = 1; i < handle_length; ++i) {
            result.push_back(Tree::Edge{static_cast<int>(i), static_cast<int>(i + 1), 1, false});
        }
        const int attach = static_cast<int>(handle_length);
        for (size_type i = handle_length + 1; i <= n; ++i) {
            result.push_back(Tree::Edge{attach, static_cast<int>(i), 1, false});
        }
        return result;
    }
};

} // namespace cp_stress_gen::anti
