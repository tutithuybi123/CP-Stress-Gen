# CP-Stress-Gen Design Notes

## Library Shape

- Header-only C++17.
- No external dependencies.
- Public umbrella header: `include/cp_stress_gen.hpp`.
- Core utilities live under `include/core/`.
- Generators live under `include/modules/`.
- Safe adversarial patterns live under `include/anti/`.

## Namespace

All APIs use `cp_stress_gen`. Core helpers use `cp_stress_gen::core`. Safe adversarial pattern helpers use `cp_stress_gen::anti`.

## Random Model

Randomized generators should use one explicit random source:

```cpp
cp_stress_gen::core::Random rng(42);
auto values = cp_stress_gen::Array(10).range(1, 100).build(rng);
```

This keeps seeded behavior deterministic and avoids competing per-generator RNG state. Convenience `build()` overloads may use time-based randomness for quick examples, but tests and reproducible workflows should pass `Random&` explicitly.

## Fluent API

Generators should prefer short fluent configuration methods:

```cpp
auto edges = cp_stress_gen::Graph(8)
    .edges(12)
    .directed()
    .dag()
    .no_multi_edges()
    .build(rng);
```

Invalid configurations should throw before producing output.

## Edge Convention

Tree and graph edge structs expose predictable fields:

```cpp
int u;
int v;
long long w;
bool weighted;
```

`weighted == false` means printers emit `u v`; `weighted == true` means printers emit `u v w`.

## Validation Policy

Use `cp_stress_gen::core::require` helpers for invalid configurations:

- invalid ranges
- impossible graph edge counts
- invalid tree centers or partition sizes
- invalid string alphabets or patterns
- impossible unique point requests

Prefer throwing `std::invalid_argument` over silently changing user intent.

## C++17 Compatibility

Do not use:

- concepts
- ranges/views
- designated initializers
- spaceship operators
- C++20 library-only APIs

Allowed:

- `if constexpr`
- `[[nodiscard]]`
- inline definitions in headers
- standard C++17 type traits

