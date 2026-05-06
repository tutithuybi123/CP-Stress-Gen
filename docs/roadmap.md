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

## Phase 4: Public Release Polish

| Status | Task | Objective | Acceptance Criteria |
| --- | --- | --- | --- |
| Done | Release notes | Add initial public release notes. | `CHANGELOG.md` includes `0.1.0` unreleased notes. |
| Done | Contribution guide | Document local setup, checks, and style rules. | `CONTRIBUTING.md` explains direct `g++` workflow. |
| Done | Issue templates | Add structured bug and feature request forms. | GitHub issue template YAML files are present. |
| Done | README badges | Show CI, license, and C++17 status. | Badges link to the existing workflow, license, and C++17 reference. |
| Done | Release consistency | Review public docs for Phase 4 release readiness. | Docs avoid unsupported feature claims. |

## Phase 5: Advanced Generators

| Status | Task | Objective | Acceptance Criteria |
| --- | --- | --- | --- |
| Done | Printer modifiers | Make output formatting more ergonomic. | Indexed views, matrix/grid helpers, testcase output, and edge headers are tested. |
| Done | Tree composition | Support custom tree construction beyond presets. | `TreeBuilder` attach/merge/relabel/shuffle paths compile and pass connectivity tests. |
| Done | Graph composition | Support custom graph construction and complements. | `GraphBuilder` path/cycle/clique/bipartite/merge/complement behavior is tested. |
| Done | Algorithm-targeted patterns | Add stress candidate shapes for common algorithm families. | Patterns have invariant tests and honest docs. |
| Done | String/hash utilities | Add string pattern helpers and rolling hash utilities. | Hash helpers and string invariants are tested. |
| Done | Geometry helpers | Add safe polygon measurements and regular polygon candidates. | Orientation, area, convexity, and invalid inputs are tested. |

## Phase 6: Gap Closing

| Status | Task | Objective | Acceptance Criteria |
| --- | --- | --- | --- |
| Planned | Markdown reformat | Keep docs readable in raw form and diffs. | Core docs are wrapped without losing meaning. |
| Planned | Single-header bundle | Let users copy one generated header. | `dist/cp_stress_gen.hpp` compiles with `-Idist`. |
| Planned | Deeper string/hash helpers | Add stronger pattern and hash utilities. | New helpers are tested and documented honestly. |
| Planned | Deeper geometry helpers | Add segment, hull, and point utilities. | Geometry invariants and invalid inputs are tested. |
| Planned | Deeper math helpers | Add number theory, modular, and combinatorics helpers. | Math helpers have known-value tests. |

## Release Checklist

- `scripts/check_all.ps1` passes locally.
- GitHub Actions CI is green.
- README badges and links render correctly.
- `CHANGELOG.md` has the intended version section.
- `LICENSE` is present and MIT.
- No generated artifacts are committed.
- Deferred work remains clearly marked.

## Deferred Work

- GUI or IDE bridge for `TestSuite-Studio`.
- CMake/package manager integration.
- Arbitrary convex polygon generation.
- Large-range prime generation.
- True hash collision generators.
- Mathematically proven Dijkstra-specific breaker generators.
- Contest-validator/interactor features comparable to `testlib.h`.
