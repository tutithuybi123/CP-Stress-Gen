# Geometry

Geometry support is currently integer-first.

```cpp
using cp_stress_gen::Point;
Point<long long> p{1, 2};
```

## Point Generators

- `Geometry::points(count).rectangle(x1, y1, x2, y2)`: random grid points.
- `.unique()`: require unique points within the rectangle capacity.
- `Geometry::collinear(count).from(x, y).step(dx, dy)`: collinear points.
- `Geometry::rectangle_boundary_points(count).rectangle(x1, y1, x2, y2)`: boundary samples.
- `Geometry::clustered_points(count).center(x, y).radius(r)`: points near a center.
- `Geometry::duplicate_points(count).point(x, y)`: repeated point.

```cpp
cp_stress_gen::core::Random rng(7);
auto unique = cp_stress_gen::Geometry::points(9)
    .rectangle(1, 1, 3, 3)
    .unique()
    .build(rng);
```

Convex polygon generation is not guaranteed yet.
