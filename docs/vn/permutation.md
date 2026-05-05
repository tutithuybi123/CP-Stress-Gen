# Hoán vị

`cp_stress_gen::Permutation(n)` tạo hoán vị của các số nguyên liên tiếp.

```cpp
cp_stress_gen::core::Random rng(3);
auto p = cp_stress_gen::Permutation(10).shuffle().build(rng);
```

## Chỉ số

- `one_based()`: giá trị `1..n`, đây là mặc định.
- `zero_based()`: giá trị `0..n-1`.

## Chế độ

- Mặc định: hoán vị đơn vị.
- `shuffle()`: hoán vị ngẫu nhiên.
- `reversed()`: thứ tự giảm dần.
- `almost_sorted(swaps)`: hoán đổi ngẫu nhiên có giới hạn.
- `cyclic_shift(k)`: xoay vòng hoán vị đơn vị.
- `few_swaps(count)`: một số ít lần hoán đổi.
- `blocks(block_size)`: đảo từng khối nhỏ nhưng vẫn giữ tính hợp lệ của hoán vị.

Số lần hoán đổi hoặc kích thước khối không hợp lệ sẽ ném ngoại lệ.
