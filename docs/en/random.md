# Random

`cp_stress_gen::core::Random` implements Xoshiro256** and is the single random source used by randomized generators.

## Basic Usage

```cpp
cp_stress_gen::core::Random rng(42);

int x = rng.integer<int>(1, 100);
double y = rng.real<double>(0.0, 1.0);
bool ok = rng.boolean(0.25);
```

## Reproducibility

Two `Random` objects with the same seed produce the same sequence.

```cpp
cp_stress_gen::core::Random a(7);
cp_stress_gen::core::Random b(7);
```

Pass the RNG into generators:

```cpp
auto s = cp_stress_gen::String(20).lowercase().build(a);
```

## Helpers

- `next_u64()` and `next_u32()` return raw random values.
- `integer(left, right)` returns an inclusive integer.
- `real(left, right)` returns a real value in `[left, right)`.
- `boolean()` returns a random bit.
- `boolean(probability_true)` samples a biased boolean.
- `jump()` and `long_jump()` advance the generator stream.
