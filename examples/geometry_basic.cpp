#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(42);
    const auto points = cp_stress_gen::Geometry::points(5).rectangle(-10, -10, 10, 10).unique().build(rng);

    cp_stress_gen::core::Printer out(std::cout);
    out.line(points.size());
    for (const auto& point : points) {
        out.line(point.x, point.y);
    }
    return 0;
}

