# Changelog

All notable changes to `CP-Stress-Gen` will be documented in this file.

## [0.3.0] - Unreleased

### Added

- Planned command-line options helper for generator scripts.
- Planned portable Polygon, Themis, and local stress-test workflow examples.
- Planned benchmark examples and practical cookbook documentation.

### Changed

- Planned package-free installation and single-header usage documentation updates.

### Notes

- No CMake, external dependencies, or `v0.3.0` tag are planned in this development
  batch.

## [0.2.0] - 2026-05-06

### Added

- Printer modifiers for indexed output, separators, matrices, grids, testcase
  blocks, and edge headers.
- Composable `TreeBuilder` and `GraphBuilder` APIs for custom structures.
- Algorithm-targeted stress candidate patterns for DFS/BFS, Dijkstra, DSU, DP,
  greedy, binary search, string/hash, and sort workflows.
- Single-header bundle generation through `scripts/bundle_single_header.ps1` and the
  committed `dist/cp_stress_gen.hpp` release artifact.
- String pattern helpers and rolling hash utilities.
- Deeper string helpers including Thue-Morse, Fibonacci words, de Bruijn sequences,
  border chains, and KMP/Z stress candidates.
- Geometry orientation, area, convexity, segment, bounding-box, hull, and regular
  polygon candidate helpers.
- Math helpers for SPF sieves, Mobius, Euler phi, CRT, modular arithmetic, small
  binomial values, factorials modulo `mod`, and random number-theory candidates.
- Raw Markdown formatting cleanup for easier review and diffs.

### Notes

- Algorithm-targeted patterns are candidate stress shapes, not guaranteed breakers.
- True hash collision construction and arbitrary integer convex polygon generation
  remain deferred.
- `include/` remains the source of truth; `dist/cp_stress_gen.hpp` is generated for
  copy-only use.

## [0.1.0] - 2026-05-05

Initial public release.

### Added

- Header-only C++17 umbrella include: `cp_stress_gen.hpp`.
- Core helpers:
  - Xoshiro256** random generator.
  - Validation helpers.
  - Printer for values, vectors, and tree/graph edges.
- Generator modules:
  - Arrays.
  - Strings.
  - Permutations.
  - Trees.
  - Graphs.
  - Math helpers.
  - Integer geometry helpers.
- Safe anti-pattern helpers for sort, tree, graph, and string shapes.
- English and Vietnamese documentation.
- API reference and design notes.
- Easy, medium, hard, and structure-focused examples.
- Assert-based tests with visible `[PASS]` output.
- Local PowerShell scripts for direct `g++` checks.
- GitHub Actions CI using direct `g++` commands.

### Notes

- No CMake or package manager integration is included yet.
- No GUI or `TestSuite-Studio` integration is included yet.
- Advanced adversarial generators such as hash collision and Dijkstra-specific
  patterns are deferred.
