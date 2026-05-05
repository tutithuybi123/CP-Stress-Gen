# CP-Stress-Gen Roadmap

## Phase 1 Priorities

| Priority | Task | Objective | Files Affected | Acceptance Criteria |
| --- | --- | --- | --- | --- |
| P0 | Scope and docs | Make the project direction explicit. | `README.md`, `docs/`, `.gitignore`, root `AGENTS.md` | Scope says CP-Stress-Gen only, C++17, header-only, direct g++ checks. |
| P0 | C++17 cleanup | Remove C++20-only features from public headers. | `include/core/Random.hpp`, `include/modules/*.hpp` | Existing generator tests compile with `-std=c++17`. |
| P0 | Validation helpers | Provide clear errors for invalid configs. | `include/core/Validate.hpp` | Invalid generator requests throw standard exceptions. |
| P0 | Printer helpers | Provide reusable output helpers. | `include/core/Printer.hpp` | Vectors and standardized edges print predictably. |
| P0 | Existing generators | Stabilize Array, Tree, and Graph. | `include/modules/Array.hpp`, `Tree.hpp`, `Graph.hpp` | Counts, bounds, connectivity, DAG order, duplicate rules covered by tests. |
| P1 | String generator | Add practical random string generation. | `include/modules/String.hpp` | Alphabet/range modes and invalid configs covered by tests. |
| P1 | Permutation generator | Add one-based/zero-based permutations. | `include/modules/Permutation.hpp` | Shuffle, reversed, and almost-sorted modes covered by tests. |
| P1 | Math helpers | Add small, safe CP math helpers. | `include/modules/Math.hpp` | gcd/lcm/divisors/primes/random_prime covered by tests. |
| P1 | Integer geometry | Add safe point generators. | `include/modules/Geometry.hpp` | Rectangle, unique grid, and collinear point tests pass. |
| P1 | Safe anti patterns | Add honest adversarial pattern modules. | `include/anti/*.hpp` | Pattern sizes and simple invariants covered by tests. |
| P1 | Examples | Add standalone usage examples. | `examples/*.cpp` | All examples compile with direct g++ commands. |
| P1 | Tests | Add framework-free tests. | `tests/*.cpp` | All tests compile and run locally. |
| P2 | Packaging notes | Document include-only usage and future install options. | `README.md`, `docs/design.md` | No package manager files required in Phase 1. |

## Deferred Work

- GUI or IDE bridge for `TestSuite-Studio`.
- CMake/package manager integration.
- Hash collision generators.
- Dijkstra-specific adversarial generators.
- Advanced geometry such as guaranteed convex polygon generation.
- Contest-validator/interactor features comparable to `testlib.h`.

