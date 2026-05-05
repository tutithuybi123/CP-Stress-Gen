# Hình học

Phần hình học hiện tập trung vào tọa độ nguyên.

```cpp
using cp_stress_gen::Point;
Point<long long> p{1, 2};
```

## Trình sinh điểm

- `Geometry::points(count).rectangle(x1, y1, x2, y2)`: điểm lưới ngẫu nhiên.
- `.unique()`: yêu cầu các điểm không trùng nhau trong hình chữ nhật.
- `Geometry::collinear(count).from(x, y).step(dx, dy)`: các điểm thẳng hàng.
- `Geometry::rectangle_boundary_points(count).rectangle(x1, y1, x2, y2)`: điểm trên biên hình chữ nhật.
- `Geometry::clustered_points(count).center(x, y).radius(r)`: điểm gần một tâm.
- `Geometry::duplicate_points(count).point(x, y)`: lặp lại cùng một điểm.

```cpp
cp_stress_gen::core::Random rng(7);
auto unique = cp_stress_gen::Geometry::points(9)
    .rectangle(1, 1, 3, 3)
    .unique()
    .build(rng);
```

Sinh đa giác lồi có bảo đảm vẫn chưa được hỗ trợ.
