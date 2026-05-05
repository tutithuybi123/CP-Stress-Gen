#include "core/Printer.hpp"

#include <iostream>
#include <vector>

struct Edge {
    int u;
    int v;
    long long w;
    bool weighted;
};

int main() {
    cp_stress_gen::core::Printer out(std::cout);
    out.line(3, 2);
    out.vector(std::vector<int>{4, 1, 7});
    out.edges(std::vector<Edge>{{1, 2, 5, true}, {2, 3, 8, true}});
    return 0;
}

