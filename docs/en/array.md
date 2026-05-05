# Array

`cp_stress_gen::Array(n)` creates integer arrays with fluent configuration.

```cpp
cp_stress_gen::core::Random rng(1);
auto a = cp_stress_gen::Array(10).range(1, 100).build(rng);
```

## Modes

- `range(l, r)`: random values in `[l, r]`.
- `fill(value)`: repeat one value.
- `iota(start, step)`: arithmetic sequence.
- `shuffle()`: shuffle the generated result.
- `almost_sorted(swaps)`: start with `1..n`, then perform bounded swaps.
- `many_equal(distinct_values)`: random values in `[1, distinct_values]`.
- `blocky(block_size)`: repeated block identifiers.

## Examples

```cpp
auto sorted_like = cp_stress_gen::Array(20).almost_sorted(3).build(rng);
auto duplicates = cp_stress_gen::Array(30).many_equal(4).build(rng);
auto blocks = cp_stress_gen::Array(12).blocky(3).build(rng);
```

Invalid ranges and invalid counts throw `std::invalid_argument`.
