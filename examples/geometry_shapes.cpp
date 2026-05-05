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
    cp_stress_gen::core::Printer out(std::cout);

    const auto rectangle = cp_stress_gen::Geometry::rectangle_polygon(0, 0, 4, 3);
    const auto triangle = cp_stress_gen::Geometry::triangle(
        cp_stress_gen::Geometry::point_type{0, 0},
        cp_stress_gen::Geometry::point_type{4, 0},
        cp_stress_gen::Geometry::point_type{1, 3}
    );

    out.line(2);
    print_points(out, rectangle);
    print_points(out, triangle);
    return 0;
}
