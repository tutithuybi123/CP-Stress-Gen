#pragma once

#include "../core/Random.hpp"
#include "../core/Validate.hpp"

#include <algorithm>
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

    [[nodiscard]] static PointGenerator points(const size_type count) {
        return PointGenerator(count);
    }

    [[nodiscard]] static CollinearGenerator collinear(const size_type count) {
        return CollinearGenerator(count);
    }
};

} // namespace cp_stress_gen

