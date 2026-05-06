# Changelog

All notable changes to `CP-Stress-Gen` will be documented in this file.

## [0.2.0] - Unreleased

### Added

- Printer modifiers for indexed output, separators, matrices, grids, testcase blocks, and edge headers.
- Composable `TreeBuilder` and `GraphBuilder` APIs for custom structures.
- Algorithm-targeted stress candidate patterns for DFS/BFS, Dijkstra, DSU, DP, greedy, binary search, string/hash, and sort workflows.
- String pattern helpers and rolling hash utilities.
- Geometry orientation, area, convexity, and regular polygon candidate helpers.

### Notes

- Algorithm-targeted patterns are candidate stress shapes, not guaranteed breakers.
- True hash collision construction and arbitrary integer convex polygon generation remain deferred.

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
- Advanced adversarial generators such as hash collision and Dijkstra-specific patterns are deferred.
