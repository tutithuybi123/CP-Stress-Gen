#include "cp_stress_gen.hpp"

#include <iostream>
#include <vector>

int main() {
    cp_stress_gen::core::Random rng(202);
    cp_stress_gen::core::Printer out(std::cout);

    const auto points = cp_stress_gen::Geometry::grid_points(3, 3);
    const auto hull = cp_stress_gen::Geometry::convex_hull(points);

    out.line(hull.size());
    for (const auto& point : hull) {
        out.line(point.x, point.y);
    }

    const auto box = cp_stress_gen::Geometry::bounding_box(points);
    out.line(box.min.x, box.min.y, box.max.x, box.max.y);

    const auto random_hull = cp_stress_gen::Geometry::convex_hull_of_random_points(20, -10, 10, rng);
    out.line(random_hull.size());
    for (const auto& point : random_hull) {
        out.line(point.x, point.y);
    }

    const auto circle = cp_stress_gen::Geometry::circle_points(6, 5.0);
    out.line(circle.size());

    return 0;
}
