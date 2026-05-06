#include "cp_stress_gen.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>

int main() {
    cp_stress_gen::core::Random rng(7);

    assert(cp_stress_gen::Geometry::points(0).rectangle(0, 0, 0, 0).build(rng).empty());
    assert(cp_stress_gen::Geometry::collinear(1).step(0, 0).build().size() == 1);

    const auto points = cp_stress_gen::Geometry::points(50).rectangle(-3, -4, 3, 4).build(rng);
    assert(points.size() == 50);
    for (const auto& point : points) {
        assert(point.x >= -3 && point.x <= 3);
        assert(point.y >= -4 && point.y <= 4);
    }

    const auto unique = cp_stress_gen::Geometry::points(9).rectangle(1, 1, 3, 3).unique().build(rng);
    assert(unique.size() == 9);
    std::set<cp_stress_gen::Point<long long>> seen(unique.begin(), unique.end());
    assert(seen.size() == unique.size());

    const auto line = cp_stress_gen::Geometry::collinear(5).from(2, 3).step(4, -1).build();
    assert(line.size() == 5);
    for (std::size_t i = 0; i < line.size(); ++i) {
        assert(line[i].x == 2 + 4 * static_cast<long long>(i));
        assert(line[i].y == 3 - static_cast<long long>(i));
    }

    const auto boundary = cp_stress_gen::Geometry::rectangle_boundary_points(20).rectangle(0, 0, 3, 2).build(rng);
    assert(boundary.size() == 20);
    for (const auto& point : boundary) {
        assert(point.x == 0 || point.x == 3 || point.y == 0 || point.y == 2);
    }

    const auto clustered = cp_stress_gen::Geometry::clustered_points(30).center(10, -10).radius(2).build(rng);
    assert(clustered.size() == 30);
    for (const auto& point : clustered) {
        assert(point.x >= 8 && point.x <= 12);
        assert(point.y >= -12 && point.y <= -8);
    }

    const auto duplicates = cp_stress_gen::Geometry::duplicate_points(5).point(4, 7).build();
    assert(duplicates.size() == 5);
    for (const auto& point : duplicates) {
        assert(point.x == 4 && point.y == 7);
    }

    const auto rectangle = cp_stress_gen::Geometry::rectangle_polygon(0, 0, 3, 2);
    assert(rectangle.size() == 4);
    assert(rectangle[0].x == 0 && rectangle[0].y == 0);
    assert(rectangle[2].x == 3 && rectangle[2].y == 2);

    const auto triangle = cp_stress_gen::Geometry::triangle(
        cp_stress_gen::Geometry::point_type{0, 0},
        cp_stress_gen::Geometry::point_type{2, 0},
        cp_stress_gen::Geometry::point_type{0, 2}
    );
    assert(triangle.size() == 3);
    assert(cp_stress_gen::Geometry::cross(triangle[0], triangle[1], triangle[2]) == 4);
    assert(cp_stress_gen::Geometry::orientation(triangle[0], triangle[1], triangle[2]) == 1);
    assert(cp_stress_gen::Geometry::orientation(triangle[0], triangle[2], triangle[1]) == -1);
    assert(cp_stress_gen::Geometry::orientation(
        cp_stress_gen::Geometry::point_type{0, 0},
        cp_stress_gen::Geometry::point_type{1, 1},
        cp_stress_gen::Geometry::point_type{2, 2}
    ) == 0);
    assert(cp_stress_gen::Geometry::dot(
        cp_stress_gen::Geometry::point_type{0, 0},
        cp_stress_gen::Geometry::point_type{2, 0},
        cp_stress_gen::Geometry::point_type{2, 2}
    ) == 4);
    assert(cp_stress_gen::Geometry::dist2(
        cp_stress_gen::Geometry::point_type{0, 0},
        cp_stress_gen::Geometry::point_type{3, 4}
    ) == 25);
    assert(cp_stress_gen::Geometry::manhattan(
        cp_stress_gen::Geometry::point_type{-1, 2},
        cp_stress_gen::Geometry::point_type{3, -4}
    ) == 10);
    assert(cp_stress_gen::Geometry::on_segment(
        cp_stress_gen::Geometry::point_type{0, 0},
        cp_stress_gen::Geometry::point_type{4, 4},
        cp_stress_gen::Geometry::point_type{2, 2}
    ));
    assert(!cp_stress_gen::Geometry::on_segment(
        cp_stress_gen::Geometry::point_type{0, 0},
        cp_stress_gen::Geometry::point_type{4, 4},
        cp_stress_gen::Geometry::point_type{5, 5}
    ));
    assert(cp_stress_gen::Geometry::segments_intersect(
        cp_stress_gen::Geometry::point_type{0, 0},
        cp_stress_gen::Geometry::point_type{4, 4},
        cp_stress_gen::Geometry::point_type{0, 4},
        cp_stress_gen::Geometry::point_type{4, 0}
    ));
    assert(!cp_stress_gen::Geometry::segments_intersect(
        cp_stress_gen::Geometry::point_type{0, 0},
        cp_stress_gen::Geometry::point_type{1, 0},
        cp_stress_gen::Geometry::point_type{0, 2},
        cp_stress_gen::Geometry::point_type{1, 2}
    ));
    assert(cp_stress_gen::Geometry::polygon_area2(rectangle) == 12);
    assert(std::fabs(cp_stress_gen::Geometry::polygon_area(rectangle) - 6.0) < 1e-9);
    assert(cp_stress_gen::Geometry::is_convex(rectangle));
    assert(cp_stress_gen::Geometry::is_simple_polygon(rectangle));
    assert(!cp_stress_gen::Geometry::is_convex(std::vector<cp_stress_gen::Geometry::point_type>{
        cp_stress_gen::Geometry::point_type{0, 0},
        cp_stress_gen::Geometry::point_type{2, 0},
        cp_stress_gen::Geometry::point_type{1, 1},
        cp_stress_gen::Geometry::point_type{2, 2},
        cp_stress_gen::Geometry::point_type{0, 2}
    }));
    assert(!cp_stress_gen::Geometry::is_simple_polygon(std::vector<cp_stress_gen::Geometry::point_type>{
        cp_stress_gen::Geometry::point_type{0, 0},
        cp_stress_gen::Geometry::point_type{2, 2},
        cp_stress_gen::Geometry::point_type{0, 2},
        cp_stress_gen::Geometry::point_type{2, 0}
    }));

    const auto box = cp_stress_gen::Geometry::bounding_box(std::vector<cp_stress_gen::Geometry::point_type>{
        cp_stress_gen::Geometry::point_type{3, -1},
        cp_stress_gen::Geometry::point_type{-2, 5},
        cp_stress_gen::Geometry::point_type{1, 4}
    });
    assert(box.min.x == -2 && box.min.y == -1);
    assert(box.max.x == 3 && box.max.y == 5);

    const auto hull = cp_stress_gen::Geometry::convex_hull(std::vector<cp_stress_gen::Geometry::point_type>{
        cp_stress_gen::Geometry::point_type{0, 0},
        cp_stress_gen::Geometry::point_type{2, 0},
        cp_stress_gen::Geometry::point_type{2, 2},
        cp_stress_gen::Geometry::point_type{0, 2},
        cp_stress_gen::Geometry::point_type{1, 1}
    });
    assert(hull.size() == 4);
    assert(cp_stress_gen::Geometry::is_convex(hull));

    const auto regular = cp_stress_gen::Geometry::regular_polygon(5, 10.0);
    assert(regular.size() == 5);
    assert(cp_stress_gen::Geometry::is_convex(regular));
    const auto candidate = cp_stress_gen::Geometry::convex_polygon_candidate(6, 3.0);
    assert(candidate.size() == 6);
    assert(cp_stress_gen::Geometry::is_convex(candidate));
    const auto circle = cp_stress_gen::Geometry::circle_points(4, 2.0);
    assert(circle.size() == 4);
    assert(std::fabs(circle[0].x - 2.0) < 1e-9);
    const auto grid = cp_stress_gen::Geometry::grid_points(2, 3);
    assert(grid.size() == 6);
    assert(grid.front().x == 0 && grid.front().y == 0);
    assert(grid.back().x == 1 && grid.back().y == 2);
    const auto random_hull = cp_stress_gen::Geometry::convex_hull_of_random_points(10, -5, 5, rng);
    assert(!random_hull.empty());

    cp_stress_gen::core::Random same_a(22);
    cp_stress_gen::core::Random same_b(22);
    assert(cp_stress_gen::Geometry::clustered_points(20).center(5, 5).radius(3).build(same_a) == cp_stress_gen::Geometry::clustered_points(20).center(5, 5).radius(3).build(same_b));

    bool thrown = false;
    try {
        (void)cp_stress_gen::Geometry::rectangle_polygon(0, 0, 0, 2);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Geometry::triangle(
            cp_stress_gen::Geometry::point_type{0, 0},
            cp_stress_gen::Geometry::point_type{1, 1},
            cp_stress_gen::Geometry::point_type{2, 2}
        );
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Geometry::points(3).rectangle(3, 1, 2, 4);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Geometry::points(10).rectangle(1, 1, 2, 2).unique().build(rng);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Geometry::collinear(3).step(0, 0).build();
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Geometry::rectangle_boundary_points(3).rectangle(2, 0, 1, 0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Geometry::clustered_points(3).radius(-1);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Geometry::polygon_area(std::vector<cp_stress_gen::Geometry::point_type>{
            cp_stress_gen::Geometry::point_type{0, 0},
            cp_stress_gen::Geometry::point_type{1, 0}
        });
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Geometry::regular_polygon(2, 1.0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Geometry::regular_polygon(3, 0.0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Geometry::bounding_box(std::vector<cp_stress_gen::Geometry::point_type>{});
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Geometry::is_simple_polygon(std::vector<cp_stress_gen::Geometry::point_type>{
            cp_stress_gen::Geometry::point_type{0, 0},
            cp_stress_gen::Geometry::point_type{1, 0}
        });
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Geometry::grid_points(0, 2);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::Geometry::circle_points(4, 0.0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "[PASS] test_geometry\n";
    return 0;
}
