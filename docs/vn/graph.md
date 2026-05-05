# Đồ thị

`cp_stress_gen::Graph(n)` sinh danh sách cạnh của đồ thị. Dùng `.edges(m)` khi cần số cạnh cụ thể.

```cpp
cp_stress_gen::core::Random rng(5);
auto edges = cp_stress_gen::Graph(8)
    .edges(12)
    .directed()
    .dag()
    .no_multi_edges()
    .build(rng);
```

## Tùy chọn chung

- `directed()` và `undirected()` cho đồ thị có hướng/vô hướng.
- `one_based()`, `zero_based()`, và `first_node(x)` để đặt nhãn đỉnh.
- `edges(m)` để yêu cầu số cạnh.
- `no_multi_edges()` hoặc `allow_multi_edges()`.
- `weight(value)` hoặc `weighted(left, right)`.
- `shuffle()` để xáo trộn thứ tự cạnh.

## Chế độ

- `dag()`: đồ thị có hướng không chu trình, cạnh đi từ nhãn nhỏ sang nhãn lớn.
- `layered_dag(layers)`: cạnh đi từ lớp trước sang lớp sau.
- `dense()`: đồ thị nhiều cạnh, không trùng cạnh.
- `sparse_connected()`: đồ thị vô hướng liên thông.
- `complete()`: đồ thị đầy đủ.
- `cycle()`: đồ thị chu trình.
- `path()`: đồ thị đường đi.
- `forest(components)`: rừng vô hướng với số thành phần cố định.
- `connected_components(k)`: đồ thị vô hướng, mỗi thành phần liên thông bên trong.
- `bipartite(left_size)`: đồ thị hai phía với kích thước phía trái cố định.

Số cạnh không thể tạo, số lớp sai, phân hoạch sai, hoặc chế độ không tương thích đều ném ngoại lệ.
