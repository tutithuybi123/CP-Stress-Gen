#include "cp_stress_gen.hpp"

#include <iostream>
#include <vector>

void print_points(cp_stress_gen::core::Printer& out, const std::vector<cp_stress_gen::Point<long long>>& points) {
    out.line(points.size());
    for (const auto& point : points) {
        out.line(point.x, point.y);
    }
}

int main() {
    cp_stress_gen::core::Random rng(202);
    cp_stress_gen::core::Printer out(std::cout);

    out.line(4);
    print_points(out, cp_stress_gen::Geometry::points(5).rectangle(-5, -5, 5, 5).build(rng));
    print_points(out, cp_stress_gen::Geometry::points(5).rectangle(1, 1, 3, 3).unique().build(rng));
    print_points(out, cp_stress_gen::Geometry::rectangle_boundary_points(6).rectangle(0, 0, 4, 3).build(rng));
    print_points(out, cp_stress_gen::Geometry::clustered_points(6).center(10, 10).radius(2).build(rng));
    return 0;
}
