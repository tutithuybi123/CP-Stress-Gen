# CP-Stress-Gen Roadmap

## Phase 1: Implemented

| Status | Task | Objective | Acceptance Criteria |
| --- | --- | --- | --- |
| Done | Scope and docs | Establish CP-Stress-Gen-only C++17 scope. | README, docs, and git hygiene are present. |
| Done | C++17 cleanup | Remove C++20-only public API usage. | Headers compile with `-std=c++17`. |
| Done | Core helpers | Add random, validation, and printer helpers. | Core tests compile and run. |
| Done | Existing generators | Stabilize Array, Tree, and Graph. | Counts, bounds, connectivity, DAG order, and duplicate rules are tested. |
| Done | String and permutation | Add practical string and permutation generators. | Feature tests compile and run. |
| Done | Math and geometry | Add small math and integer geometry helpers. | Helper tests compile and run. |
| Done | Safe anti patterns | Add honest adversarial pattern modules. | Pattern tests compile and run. |
| Done | Examples and tests | Add direct `g++` examples and assert tests. | All examples compile and tests run. |

## Phase 2: Implemented

| Status | Task | Objective | Acceptance Criteria |
| --- | --- | --- | --- |
| Done | License and docs polish | Make project usage and legal status clear. | MIT license present; README reflects implemented Phase 1. |
| Done | Practical workflows | Show real stress-generation patterns. | New examples compile with direct `g++`. |
| Done | Safe generator features | Add useful patterns without breaking APIs. | Added features are validated and tested. |
| Done | Validation hardening | Expand edge-case and failure coverage. | Invalid configs throw clear exceptions. |
| Done | Packaging notes | Explain include-path usage without build systems. | Direct include workflow is documented. |

## Phase 3: Implemented

| Status | Task | Objective | Acceptance Criteria |
| --- | --- | --- | --- |
| Done | Direct g++ scripts | Add local build/test/example scripts without CMake. | `scripts/check_all.ps1` compiles tests, runs tests, and compiles examples. |
| Done | CI workflow | Add GitHub Actions direct `g++` checks. | CI builds tests, runs tests, and builds examples on `ubuntu-latest`. |
| Done | API reference | Document public modules conservatively. | `docs/api.md` covers purpose, common API, validation, and limits. |
| Done | Safe generator helpers | Add degree-limited trees, graph structures, math helpers, geometry shapes, and honest anti patterns. | New features are covered by assert tests with visible `[PASS]` output. |
| Done | Structure examples | Add concise examples for Phase 3 helpers. | New examples compile with direct `g++`. |

## Deferred Work

- GUI or IDE bridge for `TestSuite-Studio`.
- CMake/package manager integration.
- Arbitrary convex polygon generation.
- Large-range prime generation.
- Hash collision generators.
- Dijkstra-specific adversarial generators.
- Contest-validator/interactor features comparable to `testlib.h`.
