# Permutation

`cp_stress_gen::Permutation(n)` creates a permutation of consecutive integers.

```cpp
cp_stress_gen::core::Random rng(3);
auto p = cp_stress_gen::Permutation(10).shuffle().build(rng);
```

## Indexing

- `one_based()`: values `1..n` (default).
- `zero_based()`: values `0..n-1`.

## Modes

- Default: identity permutation.
- `shuffle()`: random permutation.
- `reversed()`: descending order.
- `almost_sorted(swaps)`: bounded random swaps.
- `cyclic_shift(k)`: rotate the identity permutation.
- `few_swaps(count)`: bounded random swaps.
- `blocks(block_size)`: reverse local blocks while preserving permutation validity.

Invalid swap counts and invalid block sizes throw.
