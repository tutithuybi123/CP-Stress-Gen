# Cây

`cp_stress_gen::Tree(n)` sinh `n - 1` cạnh cho một cây hợp lệ khi `n > 0`.

```cpp
cp_stress_gen::core::Random rng(4);
auto edges = cp_stress_gen::Tree(8).random().build(rng);
```

## Dạng cây

- `bamboo()`: cây dạng chuỗi.
- `star()`: một tâm nối tới mọi đỉnh còn lại.
- `star(center)`: cây sao với nhãn tâm cụ thể.
- `random()`: mỗi đỉnh mới nối vào một đỉnh trước đó.
- `binary()`: cây nhị phân kiểu heap.
- `caterpillar(spine)`: trục chính dạng đường đi, các lá gắn vào trục.
- `deep_recursion(branches)`: chuỗi dài có thêm nhánh phụ.
- `shuffle()`: xáo trộn thứ tự cạnh.

## Nhãn và trọng số

- `one_based()` dùng nhãn `1..n`, đây là mặc định.
- `zero_based()` dùng nhãn `0..n-1`.
- `first_node(x)` đặt nhãn đầu tiên.
- `weight(value)` gán một trọng số cố định.
- `weighted(left, right)` sinh trọng số trong `[left, right]`.

Tâm sai, độ dài trục sai, số nhánh sai, hoặc khoảng trọng số sai đều ném ngoại lệ.
