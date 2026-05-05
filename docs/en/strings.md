# Strings

`cp_stress_gen::String(n)` generates strings with configurable alphabets and patterns.

```cpp
cp_stress_gen::core::Random rng(2);
auto s = cp_stress_gen::String(20).lowercase().build(rng);
```

## Alphabets

- `alphabet(chars)`: custom non-empty alphabet.
- `lowercase()`: `a-z`.
- `uppercase()`: `A-Z`.
- `digits()`: `0-9`.
- `binary()`: `0` and `1`.
- `range(left, right)`: inclusive character range.

## Patterns

- `palindrome()`: mirrored random string.
- `almost_palindrome(changes)`: mutate up to `changes` mirrored positions.
- `periodic(pattern)`: repeat a non-empty pattern.
- `prefix_heavy(prefix_char, tail_char)`: mostly one character, with a different tail.

```cpp
auto p = cp_stress_gen::String(9).alphabet("abc").palindrome().build(rng);
auto periodic = cp_stress_gen::String(12).periodic("ab").build(rng);
```

Empty alphabets, invalid ranges, empty periodic patterns, and excessive almost-palindrome changes throw.
