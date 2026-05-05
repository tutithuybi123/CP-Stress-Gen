#include "cp_stress_gen.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

struct Edge {
    int u;
    int v;
    long long w;
    bool weighted;
};

int main() {
    std::ostringstream output;
    cp_stress_gen::core::Printer printer(output);

    printer.line(3, 4, 5);
    printer.space("case", 1).line();
    printer.vector(std::vector<int>{7, 8, 9});
    printer.edges(std::vector<Edge>{{1, 2, 10, true}, {2, 3, 0, false}});

    assert(output.str() == "3 4 5\ncase 1\n7 8 9\n1 2 10\n2 3\n");

    std::ostringstream tree_output;
    cp_stress_gen::core::Printer(tree_output).edges(std::vector<cp_stress_gen::Tree::Edge>{
        cp_stress_gen::Tree::Edge{1, 2, 5, true},
        cp_stress_gen::Tree::Edge{2, 3, 1, false}
    });
    assert(tree_output.str() == "1 2 5\n2 3\n");

    std::ostringstream graph_output;
    cp_stress_gen::core::Printer(graph_output).edges(std::vector<cp_stress_gen::Graph::Edge>{
        cp_stress_gen::Graph::Edge{0, 1, 9, true},
        cp_stress_gen::Graph::Edge{1, 2, 1, false}
    });
    assert(graph_output.str() == "0 1 9\n1 2\n");

    bool thrown = false;
    try {
        cp_stress_gen::core::require(false, "expected failure");
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        cp_stress_gen::core::require_range(10, 1, "bad range");
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        cp_stress_gen::core::require_positive(0, "not positive");
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "[PASS] test_printer\n";
    return 0;
}
