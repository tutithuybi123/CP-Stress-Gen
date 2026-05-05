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
};

} // namespace cp_stress_gen::anti

