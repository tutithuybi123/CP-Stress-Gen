#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <set>
#include <utility>
#include <vector>

namespace cp_stress_gen {

template <typename T>
struct Point {
    T x;
    T y;

    bool operator==(const Point& other) const noexcept {
        return x == other.x && y == other.y;
    }

    bool operator<(const Point& other) const noexcept {
        return x < other.x || (x == other.x && y < other.y);
    }
};

class Geometry {
public:
    using coord_type = long long;
    using point_type = Point<coord_type>;
    using size_type = std::size_t;

    class PointGenerator {
    public:
        explicit PointGenerator(const size_type count) : count_(count) {}

        PointGenerator& rectangle(const coord_type x1, const coord_type y1, const coord_type x2, const coord_type y2) {
            core::require_range(x1, x2, "Geometry::points rectangle requires x1 <= x2");
            core::require_range(y1, y2, "Geometry::points rectangle requires y1 <= y2");
            x1_ = x1;
            y1_ = y1;
            x2_ = x2;
            y2_ = y2;
            return *this;
        }

        PointGenerator& unique() noexcept {
            unique_ = true;
            return *this;
        }

        [[nodiscard]] std::vector<point_type> build(core::Random& rng) const {
            if (unique_) {
                return build_unique(rng);
            }
            return build_random(rng);
        }

        [[nodiscard]] std::vector<point_type> build() const {
            core::Random rng = core::Random::from_time();
            return build(rng);
        }

    private:
        size_type count_;
        coord_type x1_{0};
        coord_type y1_{0};
        coord_type x2_{0};
        coord_type y2_{0};
        bool unique_{false};

        [[nodiscard]] std::uint64_t area() const {
            const std::uint64_t width = static_cast<std::uint64_t>(x2_ - x1_ + 1);
            const std::uint64_t height = static_cast<std::uint64_t>(y2_ - y1_ + 1);
            return width * height;
        }

        [[nodiscard]] std::vector<point_type> build_random(core::Random& rng) const {
            std::vector<point_type> result;
            result.reserve(count_);
            for (size_type i = 0; i < count_; ++i) {
                result.push_back(point_type{rng.integer<coord_type>(x1_, x2_), rng.integer<coord_type>(y1_, y2_)});
            }
            return result;
        }

        [[nodiscard]] std::vector<point_type> build_unique(core::Random& rng) const {
            core::require(static_cast<std::uint64_t>(count_) <= area(), "Geometry::points unique request exceeds rectangle capacity");

            std::vector<point_type> result;
            result.reserve(count_);

            if (area() <= 1000000ull) {
                std::vector<point_type> all;
                for (coord_type x = x1_; x <= x2_; ++x) {
                    for (coord_type y = y1_; y <= y2_; ++y) {
                        all.push_back(point_type{x, y});
                    }
                }
                shuffle_points(all, rng);
                result.insert(result.end(), all.begin(), all.begin() + static_cast<std::ptrdiff_t>(count_));
                return result;
            }

            std::set<point_type> seen;
            while (result.size() < count_) {
                const point_type p{rng.integer<coord_type>(x1_, x2_), rng.integer<coord_type>(y1_, y2_)};
                if (seen.insert(p).second) {
                    result.push_back(p);
                }
            }
            return result;
        }

        static void shuffle_points(std::vector<point_type>& values, core::Random& rng) {
            if (values.size() < 2) {
                return;
            }
            for (size_type i = values.size() - 1; i > 0; --i) {
                const size_type j = rng.integer<size_type>(0, i);
                std::swap(values[i], values[j]);
            }
        }
    };

    class CollinearGenerator {
    public:
        explicit CollinearGenerator(const size_type count) : count_(count) {}

        CollinearGenerator& from(const coord_type x, const coord_type y) noexcept {
            start_ = point_type{x, y};
            return *this;
        }

        CollinearGenerator& from(const point_type point) noexcept {
            start_ = point;
            return *this;
        }

        CollinearGenerator& step(const coord_type dx, const coord_type dy) noexcept {
            dx_ = dx;
            dy_ = dy;
            return *this;
        }

        [[nodiscard]] std::vector<point_type> build() const {
            core::require(count_ <= 1 || dx_ != 0 || dy_ != 0, "Geometry::collinear step cannot be (0, 0) for multiple points");
            std::vector<point_type> result;
            result.reserve(count_);
            for (size_type i = 0; i < count_; ++i) {
                const coord_type k = static_cast<coord_type>(i);
                result.push_back(point_type{start_.x + dx_ * k, start_.y + dy_ * k});
            }
            return result;
        }

    private:
        size_type count_;
        point_type start_{0, 0};
        coord_type dx_{1};
        coord_type dy_{0};
    };

    class BoundaryGenerator {
    public:
        explicit BoundaryGenerator(const size_type count) : count_(count) {}

        BoundaryGenerator& rectangle(const coord_type x1, const coord_type y1, const coord_type x2, const coord_type y2) {
            core::require_range(x1, x2, "Geometry::rectangle_boundary_points requires x1 <= x2");
            core::require_range(y1, y2, "Geometry::rectangle_boundary_points requires y1 <= y2");
            x1_ = x1;
            y1_ = y1;
            x2_ = x2;
            y2_ = y2;
            return *this;
        }

        [[nodiscard]] std::vector<point_type> build(core::Random& rng) const {
            const std::vector<point_type> boundary = boundary_points();
            core::require(!boundary.empty() || count_ == 0, "Geometry::rectangle_boundary_points has no boundary points");

            std::vector<point_type> result;
            result.reserve(count_);
            for (size_type i = 0; i < count_; ++i) {
                const size_type index = rng.integer<size_type>(0, boundary.size() - 1);
                result.push_back(boundary[index]);
            }
            return result;
        }

        [[nodiscard]] std::vector<point_type> build() const {
            core::Random rng = core::Random::from_time();
            return build(rng);
        }

    private:
        size_type count_;
        coord_type x1_{0};
        coord_type y1_{0};
        coord_type x2_{0};
        coord_type y2_{0};

        [[nodiscard]] std::vector<point_type> boundary_points() const {
            std::vector<point_type> result;
            for (coord_type x = x1_; x <= x2_; ++x) {
                result.push_back(point_type{x, y1_});
                if (y2_ != y1_) {
                    result.push_back(point_type{x, y2_});
                }
            }
            for (coord_type y = y1_ + 1; y < y2_; ++y) {
                result.push_back(point_type{x1_, y});
                if (x2_ != x1_) {
                    result.push_back(point_type{x2_, y});
                }
            }
            return result;
        }
    };

    class ClusterGenerator {
    public:
        explicit ClusterGenerator(const size_type count) : count_(count) {}

        ClusterGenerator& center(const coord_type x, const coord_type y) noexcept {
            center_ = point_type{x, y};
            return *this;
        }

        ClusterGenerator& radius(const coord_type radius) {
            core::require(radius >= 0, "Geometry::clustered_points radius must be non-negative");
            radius_ = radius;
            return *this;
        }

        [[nodiscard]] std::vector<point_type> build(core::Random& rng) const {
            std::vector<point_type> result;
            result.reserve(count_);
            for (size_type i = 0; i < count_; ++i) {
                result.push_back(point_type{
                    rng.integer<coord_type>(center_.x - radius_, center_.x + radius_),
                    rng.integer<coord_type>(center_.y - radius_, center_.y + radius_)
                });
            }
            return result;
        }

        [[nodiscard]] std::vector<point_type> build() const {
            core::Random rng = core::Random::from_time();
            return build(rng);
        }

    private:
        size_type count_;
        point_type center_{0, 0};
        coord_type radius_{0};
    };

    class DuplicateGenerator {
    public:
        explicit DuplicateGenerator(const size_type count) : count_(count) {}

        DuplicateGenerator& point(const coord_type x, const coord_type y) noexcept {
            point_ = point_type{x, y};
            return *this;
        }

        DuplicateGenerator& point(const point_type point) noexcept {
            point_ = point;
            return *this;
        }

        [[nodiscard]] std::vector<point_type> build() const {
            return std::vector<point_type>(count_, point_);
        }

    private:
        size_type count_;
        point_type point_{0, 0};
    };

    [[nodiscard]] static PointGenerator points(const size_type count) {
        return PointGenerator(count);
    }

    [[nodiscard]] static CollinearGenerator collinear(const size_type count) {
        return CollinearGenerator(count);
    }

    [[nodiscard]] static BoundaryGenerator rectangle_boundary_points(const size_type count) {
        return BoundaryGenerator(count);
    }

    [[nodiscard]] static ClusterGenerator clustered_points(const size_type count) {
        return ClusterGenerator(count);
    }

    [[nodiscard]] static DuplicateGenerator duplicate_points(const size_type count) {
        return DuplicateGenerator(count);
    }

    [[nodiscard]] static std::vector<point_type> rectangle_polygon(
        const coord_type x1,
        const coord_type y1,
        const coord_type x2,
        const coord_type y2
    ) {
        core::require(x1 < x2, "Geometry::rectangle_polygon requires x1 < x2");
        core::require(y1 < y2, "Geometry::rectangle_polygon requires y1 < y2");
        return std::vector<point_type>{
            point_type{x1, y1},
            point_type{x2, y1},
            point_type{x2, y2},
            point_type{x1, y2}
        };
    }

    [[nodiscard]] static std::vector<point_type> triangle(
        const point_type a,
        const point_type b,
        const point_type c
    ) {
        core::require(cross(a, b, c) != 0, "Geometry::triangle requires non-collinear points");
        return std::vector<point_type>{a, b, c};
    }

    [[nodiscard]] static coord_type cross(const point_type a, const point_type b, const point_type c) noexcept {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    [[nodiscard]] static int orientation(const point_type a, const point_type b, const point_type c) noexcept {
        const coord_type value = cross(a, b, c);
        if (value > 0) {
            return 1;
        }
        if (value < 0) {
            return -1;
        }
        return 0;
    }

    [[nodiscard]] static coord_type polygon_area2(const std::vector<point_type>& polygon) {
        core::require(polygon.size() >= 3, "Geometry::polygon_area2 requires at least three points");
        coord_type result = 0;
        for (size_type i = 0; i < polygon.size(); ++i) {
            const point_type a = polygon[i];
            const point_type b = polygon[(i + 1) % polygon.size()];
            result += a.x * b.y - a.y * b.x;
        }
        return result;
    }

    [[nodiscard]] static double polygon_area(const std::vector<point_type>& polygon) {
        return std::fabs(static_cast<double>(polygon_area2(polygon))) / 2.0;
    }

    [[nodiscard]] static bool is_convex(const std::vector<point_type>& polygon) {
        core::require(polygon.size() >= 3, "Geometry::is_convex requires at least three points");
        int sign = 0;
        for (size_type i = 0; i < polygon.size(); ++i) {
            const int current = orientation(polygon[i], polygon[(i + 1) % polygon.size()], polygon[(i + 2) % polygon.size()]);
            if (current == 0) {
                continue;
            }
            if (sign == 0) {
                sign = current;
            } else if (sign != current) {
                return false;
            }
        }
        return sign != 0;
    }

    template <typename T>
    [[nodiscard]] static bool is_convex(const std::vector<Point<T>>& polygon) {
        core::require(polygon.size() >= 3, "Geometry::is_convex requires at least three points");
        int sign = 0;
        for (size_type i = 0; i < polygon.size(); ++i) {
            const long double value = cross_value(polygon[i], polygon[(i + 1) % polygon.size()], polygon[(i + 2) % polygon.size()]);
            const int current = value > 0 ? 1 : (value < 0 ? -1 : 0);
            if (current == 0) {
                continue;
            }
            if (sign == 0) {
                sign = current;
            } else if (sign != current) {
                return false;
            }
        }
        return sign != 0;
    }

    [[nodiscard]] static std::vector<Point<double>> regular_polygon(const size_type n, const double radius) {
        core::require(n >= 3, "Geometry::regular_polygon requires at least three points");
        core::require(radius > 0.0, "Geometry::regular_polygon radius must be positive");
        const double pi = std::acos(-1.0);
        std::vector<Point<double>> result;
        result.reserve(n);
        for (size_type i = 0; i < n; ++i) {
            const double angle = 2.0 * pi * static_cast<double>(i) / static_cast<double>(n);
            result.push_back(Point<double>{radius * std::cos(angle), radius * std::sin(angle)});
        }
        return result;
    }

    [[nodiscard]] static std::vector<Point<double>> convex_polygon_candidate(const size_type n, const double radius) {
        return regular_polygon(n, radius);
    }

private:
    template <typename T>
    [[nodiscard]] static long double cross_value(const Point<T> a, const Point<T> b, const Point<T> c) noexcept {
        return (static_cast<long double>(b.x) - static_cast<long double>(a.x)) *
            (static_cast<long double>(c.y) - static_cast<long double>(a.y)) -
            (static_cast<long double>(b.y) - static_cast<long double>(a.y)) *
            (static_cast<long double>(c.x) - static_cast<long double>(a.x));
    }
};

} // namespace cp_stress_gen
