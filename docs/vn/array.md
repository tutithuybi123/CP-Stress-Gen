# Mảng

`cp_stress_gen::Array(n)` tạo mảng số nguyên bằng API dạng fluent.

```cpp
cp_stress_gen::core::Random rng(1);
auto a = cp_stress_gen::Array(10).range(1, 100).build(rng);
```

## Chế độ

- `range(l, r)`: giá trị ngẫu nhiên trong `[l, r]`.
- `fill(value)`: lặp lại một giá trị.
- `iota(start, step)`: cấp số cộng.
- `shuffle()`: xáo trộn kết quả đã sinh.
- `almost_sorted(swaps)`: bắt đầu từ `1..n`, sau đó hoán đổi có giới hạn.
- `many_equal(distinct_values)`: giá trị ngẫu nhiên trong `[1, distinct_values]`.
- `blocky(block_size)`: tạo các khối giá trị lặp.

## Ví dụ

```cpp
auto sorted_like = cp_stress_gen::Array(20).almost_sorted(3).build(rng);
auto duplicates = cp_stress_gen::Array(30).many_equal(4).build(rng);
auto blocks = cp_stress_gen::Array(12).blocky(3).build(rng);
```

Khoảng giá trị sai hoặc tham số đếm không hợp lệ sẽ ném `std::invalid_argument`.
