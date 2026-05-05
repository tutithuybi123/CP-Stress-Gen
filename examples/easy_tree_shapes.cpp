#include "cp_stress_gen.hpp"

#include <iostream>
#include <vector>

void print_tree(cp_stress_gen::core::Printer& out, const int n, const std::vector<cp_stress_gen::Tree::Edge>& edges) {
    out.line(n);
    out.edges(edges);
}

int main() {
    cp_stress_gen::core::Random rng(103);
    cp_stress_gen::core::Printer out(std::cout);

    out.line(3);
    print_tree(out, 6, cp_stress_gen::Tree(6).bamboo().build(rng));
    print_tree(out, 6, cp_stress_gen::Tree(6).star().build(rng));
    print_tree(out, 6, cp_stress_gen::Tree(6).random().build(rng));
    return 0;
}
