#include "cp_stress_gen.hpp"

#include <cassert>
#include <set>
#include <stdexcept>

int main() {
    cp_stress_gen::core::Random rng(7);

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

    bool thrown = false;
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

    return 0;
}

