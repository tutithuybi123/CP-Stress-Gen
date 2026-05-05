# Chuỗi

`cp_stress_gen::String(n)` sinh chuỗi với alphabet và mẫu có thể cấu hình.

```cpp
cp_stress_gen::core::Random rng(2);
auto s = cp_stress_gen::String(20).lowercase().build(rng);
```

## Alphabet

- `alphabet(chars)`: alphabet tùy chọn, không được rỗng.
- `lowercase()`: các ký tự `a-z`.
- `uppercase()`: các ký tự `A-Z`.
- `digits()`: các ký tự `0-9`.
- `binary()`: hai ký tự `0` và `1`.
- `range(left, right)`: đoạn ký tự đóng.

## Mẫu chuỗi

- `palindrome()`: chuỗi đối xứng.
- `almost_palindrome(changes)`: thay đổi một số vị trí đối xứng.
- `periodic(pattern)`: lặp lại một mẫu không rỗng.
- `prefix_heavy(prefix_char, tail_char)`: phần lớn là một ký tự, đuôi là ký tự khác.

```cpp
auto p = cp_stress_gen::String(9).alphabet("abc").palindrome().build(rng);
auto periodic = cp_stress_gen::String(12).periodic("ab").build(rng);
```

Alphabet rỗng, khoảng ký tự sai, mẫu tuần hoàn rỗng, hoặc số thay đổi quá lớn đều ném ngoại lệ.
