# Sinh số ngẫu nhiên

`cp_stress_gen::core::Random` cài đặt Xoshiro256** và là nguồn ngẫu nhiên chính cho các trình sinh dữ liệu.

## Cách dùng cơ bản

```cpp
cp_stress_gen::core::Random rng(42);

int x = rng.integer<int>(1, 100);
double y = rng.real<double>(0.0, 1.0);
bool ok = rng.boolean(0.25);
```

## Tái lập kết quả

Hai đối tượng `Random` có cùng seed sẽ sinh cùng một dãy giá trị.

```cpp
cp_stress_gen::core::Random a(7);
cp_stress_gen::core::Random b(7);
```

Truyền RNG vào trình sinh dữ liệu:

```cpp
auto s = cp_stress_gen::String(20).lowercase().build(a);
```

## Hàm hỗ trợ

- `next_u64()` và `next_u32()` trả về giá trị ngẫu nhiên thô.
- `integer(left, right)` sinh số nguyên trong đoạn đóng.
- `real(left, right)` sinh số thực trong `[left, right)`.
- `boolean()` sinh giá trị đúng/sai ngẫu nhiên.
- `boolean(probability_true)` sinh đúng/sai theo xác suất.
- `jump()` và `long_jump()` nhảy sang vị trí xa hơn trong luồng sinh số.
