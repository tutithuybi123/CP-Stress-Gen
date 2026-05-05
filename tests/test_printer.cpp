#include "core/Printer.hpp"
#include "core/Validate.hpp"

#include <cassert>
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

    return 0;
}

