# CP-Stress-Gen Roadmap

## Phase 1: Implemented

- Scope and docs:
  establish CP-Stress-Gen-only C++17 scope.
  Acceptance: README, docs, and git hygiene are present.
- C++17 cleanup:
  remove C++20-only public API usage.
  Acceptance: headers compile with `-std=c++17`.
- Core helpers:
  add random, validation, and printer helpers.
  Acceptance: core tests compile and run.
- Existing generators:
  stabilize Array, Tree, and Graph.
  Acceptance: counts, bounds, connectivity, DAG order, and duplicate rules are tested.
- String and permutation:
  add practical string and permutation generators.
  Acceptance: feature tests compile and run.
- Math and geometry:
  add small math and integer geometry helpers.
  Acceptance: helper tests compile and run.
- Safe anti patterns:
  add honest adversarial pattern modules.
  Acceptance: pattern tests compile and run.
- Examples and tests:
  add direct `g++` examples and assert tests.
  Acceptance: all examples compile and tests run.

## Phase 2: Implemented

- License and docs polish:
  make project usage and legal status clear.
  Acceptance: MIT license present; README reflects implemented Phase 1.
- Practical workflows:
  show real stress-generation patterns.
  Acceptance: new examples compile with direct `g++`.
- Safe generator features:
  add useful patterns without breaking APIs.
  Acceptance: added features are validated and tested.
- Validation hardening:
  expand edge-case and failure coverage.
  Acceptance: invalid configs throw clear exceptions.
- Packaging notes:
  explain include-path usage without build systems.
  Acceptance: direct include workflow is documented.

## Phase 3: Implemented

- Direct g++ scripts:
  add local build/test/example scripts without CMake.
  Acceptance: `scripts/check_all.ps1` compiles tests, runs tests, and compiles examples.
- CI workflow:
  add GitHub Actions direct `g++` checks.
  Acceptance: CI builds tests, runs tests, and builds examples on `ubuntu-latest`.
- API reference:
  document public modules conservatively.
  Acceptance: `docs/api.md` covers purpose, common API, validation, and limits.
- Safe generator helpers:
  add degree-limited trees, graph structures, math helpers, geometry shapes, and honest
  anti patterns.
  Acceptance: new features have assert tests with visible `[PASS]` output.
- Structure examples:
  add concise examples for Phase 3 helpers.
  Acceptance: new examples compile with direct `g++`.

## Phase 4: Public Release Polish

- Release notes:
  add initial public release notes.
  Acceptance: `CHANGELOG.md` includes `0.1.0` release notes.
- Contribution guide:
  document local setup, checks, and style rules.
  Acceptance: `CONTRIBUTING.md` explains direct `g++` workflow.
- Issue templates:
  add structured bug and feature request forms.
  Acceptance: GitHub issue template YAML files are present.
- README badges:
  show CI, license, and C++17 status.
  Acceptance: badges link to the existing workflow, license, and C++17 reference.
- Release consistency:
  review public docs for Phase 4 release readiness.
  Acceptance: docs avoid unsupported feature claims.

## Phase 5: Advanced Generators

- Printer modifiers:
  make output formatting more ergonomic.
  Acceptance: indexed views, matrix/grid helpers, testcase output, and edge headers are tested.
- Tree composition:
  support custom tree construction beyond presets.
  Acceptance: `TreeBuilder` attach, merge, relabel, and shuffle paths pass connectivity tests.
- Graph composition:
  support custom graph construction and complements.
  Acceptance: `GraphBuilder` path, cycle, clique, bipartite, merge, and complement
  behavior is tested.
- Algorithm-targeted patterns:
  add stress candidate shapes for common algorithm families.
  Acceptance: patterns have invariant tests and honest docs.
- String/hash utilities:
  add string pattern helpers and rolling hash utilities.
  Acceptance: hash helpers and string invariants are tested.
- Geometry helpers:
  add safe polygon measurements and regular polygon candidates.
  Acceptance: orientation, area, convexity, and invalid inputs are tested.

## Phase 6: Gap Closing

- Markdown reformat:
  keep docs readable in raw form and diffs.
  Acceptance: core docs are wrapped without losing meaning.
- Single-header bundle:
  let users copy one generated header.
  Acceptance: `dist/cp_stress_gen.hpp` compiles with `-Idist`.
- Deeper string/hash helpers:
  add stronger pattern and hash utilities.
  Acceptance: new helpers are tested and documented honestly.
- Deeper geometry helpers:
  add segment, hull, and point utilities.
  Acceptance: geometry invariants and invalid inputs are tested.
- Deeper math helpers:
  add number theory, modular, and combinatorics helpers.
  Acceptance: math helpers have known-value tests.

## Release Checklist

- `scripts/check_all.ps1` passes locally.
- GitHub Actions CI is green.
- README badges and links render correctly.
- `CHANGELOG.md` has the intended version section.
- `LICENSE` is present and MIT.
- `dist/cp_stress_gen.hpp` is regenerated when public headers change.
- No local build artifacts are committed.
- Deferred work remains clearly marked.
- For `v0.2.0`, Phase 5 and Phase 6 notes remain under `Unreleased` until the tag is
  explicitly created.

## Deferred Work

- GUI or IDE bridge for `TestSuite-Studio`.
- CMake/package manager integration.
- Arbitrary convex polygon generation.
- Large-range prime generation.
- True hash collision generators.
- Mathematically proven Dijkstra-specific breaker generators.
- Contest-validator/interactor features comparable to `testlib.h`.
