#include "cp_stress_gen.hpp"

#include <iostream>
#include <vector>

int main() {
    cp_stress_gen::core::Printer out(std::cout);

    std::vector<int> values{0, 1, 2, 3};
    out.with_n(values);
    out.indexed(1).vector(values);

    std::vector<std::vector<int>> matrix{{1, 2, 3}, {4, 5, 6}};
    out.matrix(matrix);

    std::vector<std::string> grid{"..#", "#.."};
    out.grid(grid);

    std::vector<cp_stress_gen::Graph::Edge> edges{
        cp_stress_gen::Graph::Edge{0, 1, 1, false},
        cp_stress_gen::Graph::Edge{1, 2, 5, true}
    };
    out.indexed(1).edges(3, edges);

    out.testcases(2, [](const std::size_t case_index, cp_stress_gen::core::Printer& printer) {
        printer.line("case", case_index);
    });

    return 0;
}
