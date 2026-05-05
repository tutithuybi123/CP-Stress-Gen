# Overview

`CP-Stress-Gen` is a small, header-only C++17 library for competitive programming test generation. It focuses on readable fluent APIs, deterministic output when a seed is provided, and clear failures for invalid generator configurations.

## What It Provides

| Area | Main API |
| --- | --- |
| Random | `cp_stress_gen::core::Random` |
| Output | `cp_stress_gen::core::Printer` |
| Arrays | `cp_stress_gen::Array` |
| Strings | `cp_stress_gen::String` |
| Permutations | `cp_stress_gen::Permutation` |
| Trees | `cp_stress_gen::Tree` |
| Graphs | `cp_stress_gen::Graph` |
| Math | `cp_stress_gen::Math` |
| Geometry | `cp_stress_gen::Geometry`, `cp_stress_gen::Point<T>` |
| Safe anti patterns | `cp_stress_gen::anti::*Killer` |

## Design Goals

- Header-only usage with `#include "cp_stress_gen.hpp"`.
- C++17 baseline and no external dependencies.
- Explicit seeded randomness through `build(rng)`.
- Practical generator shapes used in local stress testing.
- Exceptions for invalid configurations instead of silently changing output.

## Known Limits

`CP-Stress-Gen` is not a full replacement for validator/interactor libraries. It does not currently provide CMake/package-manager integration, GUI tooling, convex polygon guarantees, large-range prime generation, hash-collision generators, or Dijkstra-specific adversarial generators.

## Next Reading

- [Installation](installation.md)
- [Getting Started](getting_started.md)
- [Examples](examples.md)
- [Graph](graph.md)
- [Tree](tree.md)
