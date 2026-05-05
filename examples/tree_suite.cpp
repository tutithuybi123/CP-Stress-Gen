#include "cp_stress_gen.hpp"

#include <iostream>

void print_tree(cp_stress_gen::core::Printer& out, const int n, const std::vector<cp_stress_gen::Tree::Edge>& edges) {
    out.line(n);
    out.edges(edges);
}

int main() {
    cp_stress_gen::core::Random rng(42);
    cp_stress_gen::core::Printer out(std::cout);

    const auto bamboo = cp_stress_gen::Tree(8).bamboo().build(rng);
    const auto star = cp_stress_gen::Tree(8).star().build(rng);
    const auto random = cp_stress_gen::Tree(8).random().build(rng);
    const auto caterpillar = cp_stress_gen::Tree(8).caterpillar(4).build(rng);
    const auto deep = cp_stress_gen::Tree(8).deep_recursion(2).build(rng);

    out.line(5);
    print_tree(out, 8, bamboo);
    print_tree(out, 8, star);
    print_tree(out, 8, random);
    print_tree(out, 8, caterpillar);
    print_tree(out, 8, deep);
    return 0;
}

