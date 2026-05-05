# Toán học

`cp_stress_gen::Math` cung cấp các hàm nhỏ thường dùng trong trình sinh dữ liệu.

## Hàm hiện có

- `gcd(a, b)`: ước chung lớn nhất.
- `lcm(a, b)`: bội chung nhỏ nhất.
- `divisors(n)`: các ước dương của `abs(n)` theo thứ tự tăng dần.
- `primes_up_to(n)`: sàng số nguyên tố tới `n`.
- `is_prime(n)`: kiểm tra nguyên tố bằng chia thử.
- `random_prime(left, right, rng)`: chọn số nguyên tố ngẫu nhiên trong đoạn.
- `random_composite(left, right, rng)`: chọn hợp số ngẫu nhiên trong đoạn.
- `coprime_pair(left, right, rng)`: cặp số có gcd bằng `1`.
- `with_gcd(g, multiplier_left, multiplier_right, rng)`: cặp số có gcd đúng bằng `g` nếu tìm được cặp hệ số phù hợp.

```cpp
cp_stress_gen::core::Random rng(6);
int p = cp_stress_gen::Math::random_prime(10, 100, rng);
auto pair = cp_stress_gen::Math::with_gcd(6, 1, 20, rng);
```

Sinh số nguyên tố trong khoảng rất lớn vẫn là phần để làm sau.
