# CP-Stress-Gen Design Notes

## Library Shape

- Header-only C++17.
- No external dependencies.
- Public umbrella header: `include/cp_stress_gen.hpp`.
- Core utilities live under `include/core/`.
- Generators live under `include/modules/`.
- Safe adversarial patterns live under `include/anti/`.

## Namespace

All APIs use `cp_stress_gen`. Core helpers use `cp_stress_gen::core`.

## Random Model

Randomized generators should use one explicit random source:

```cpp
cp_stress_gen::core::Random rng(42);
auto values = cp_stress_gen::Array(10).range(1, 100).build(rng);
```

Avoid per-generator `seed()` and `seeded()` APIs in the current public batch. This keeps deterministic behavior easy to reason about and avoids competing random states.

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

Invalid configurations should throw clear exceptions before producing output.

## Edge Convention

Tree and graph edge structs should expose predictable fields:

```cpp
int u;
int v;
long long w;
bool weighted;
```

`weighted == false` means printers may emit only `u v`; `weighted == true` means printers emit `u v w`.

## Validation Policy

Use `cp_stress_gen::core::require` helpers for invalid configurations:

- invalid ranges
- impossible edge counts
- invalid tree centers or partition sizes
- impossible unique point requests

Prefer throwing `std::invalid_argument` over silently fixing the request.

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

