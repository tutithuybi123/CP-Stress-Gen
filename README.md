# CP-Stress-Gen

[![CI][ci-badge]][ci-workflow]
[![License: MIT][license-badge]](LICENSE)
[![C++17][cpp17-badge]][cpp17-reference]

`CP-Stress-Gen` is a lightweight, header-only C++17 library for competitive programming
test generation. It provides fluent preset generators, composable tree/graph builders,
deterministic seeded output, validation helpers, ergonomic printers, and practical
examples for local stress testing.

## Documentation

- [English documentation](docs/en/overview.md)
- [English installation guide](docs/en/installation.md)
- [API reference](docs/api.md)
- [Changelog](CHANGELOG.md)
- [Contributing](CONTRIBUTING.md)
- [Tài liệu tiếng Việt](docs/vn/overview.md)
- [Tài liệu cài đặt tiếng Việt](docs/vn/installation.md)

## Requirements

- C++17 or newer
- A standard C++ compiler such as `g++`
- No external libraries

## Quick Start

```cpp
#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(123);
    auto values = cp_stress_gen::Array(5).range(1, 10).build(rng);

    cp_stress_gen::core::Printer out(std::cout);
    out.line(values.size());
    out.vector(values);
}
```

Compile directly:

```powershell
New-Item -ItemType Directory -Force .tmp_build
g++ -std=c++17 -Wall -Wextra -pedantic `
    -Iinclude examples/easy_array_sum.cpp `
    -o .tmp_build/easy_array_sum.exe
.\.tmp_build\easy_array_sum.exe
```

## Single-Header Usage

The public source of truth is the `include/` directory. For copy-only workflows,
the repository also ships a generated bundle:

```cpp
#include "cp_stress_gen.hpp"
```

Compile with the generated header:

```powershell
.\scripts\bundle_single_header.ps1
g++ -std=c++17 -Wall -Wextra -pedantic `
    -Idist examples/single_header_basic.cpp `
    -o .tmp_build/single_header_basic.exe
```

Do not edit `dist/cp_stress_gen.hpp` manually. Regenerate it from `include/`.

## Examples

Easy:

- `examples/easy_array_sum.cpp`
- `examples/easy_string_palindrome.cpp`
- `examples/easy_tree_shapes.cpp`

Medium:

- `examples/medium_graph_cases.cpp`
- `examples/medium_geometry_points.cpp`
- `examples/medium_batch_tests.cpp`

Hard:

- `examples/hard_stress_compare.cpp`
- `examples/hard_graph_suite.cpp`
- `examples/hard_anti_patterns.cpp`

Advanced and structure examples:

- `examples/single_header_basic.cpp`
- `examples/tree_degree_limit.cpp`
- `examples/tree_composition.cpp`
- `examples/graph_structures.cpp`
- `examples/graph_composition.cpp`
- `examples/math_helpers.cpp`
- `examples/geometry_shapes.cpp`
- `examples/anti_patterns.cpp`
- `examples/options_basic.cpp`
- `examples/printer_modifiers.cpp`
- `examples/algorithm_killers.cpp`
- `examples/string_deep_patterns.cpp`
- `examples/hash_patterns.cpp`
- `examples/geometry_deep.cpp`
- `examples/math_deep.cpp`

Existing focused examples remain available in `examples/`. See [English
examples](docs/en/examples.md) or [ví dụ tiếng Việt](docs/vn/examples.md) for compile
commands and usage notes.

## Current Modules

| Area | Capabilities |
| --- | --- |
| Core | Random, validation, options, printer modifiers, vector/matrix/grid/edge output |
| Arrays | Range, fill, iota, shuffle, almost sorted, many equal, blocky |
| Strings | Alphabets, palindrome, periodic, Thue-Morse, Fibonacci, de Bruijn, hashes |
| Permutations | One/zero-based labels, shuffle, reversed, cyclic shifts, few swaps, blocks |
| Trees | Presets, weights, degree limits, `TreeBuilder` composition |
| Graphs | Presets, DAGs, components, bipartite graphs, `GraphBuilder` composition |
| Math | gcd/lcm, primes, SPF, phi, Mobius, CRT, modular math, combinatorics |
| Geometry | Points, segments, convex hulls, bounding boxes, polygon helpers |
| Anti patterns | Sort/tree/graph/string patterns and algorithm stress candidates |

## Known Limitations

- No CMake or package manager integration yet.
- No GUI or `TestSuite-Studio` bridge yet.
- No guaranteed convex polygon generator yet.
- Large-range prime generation is intentionally simple.
- Advanced adversarial generators such as true hash collision construction remain deferred;
  algorithm-targeted patterns are stress candidates, not guaranteed breakers.

## Comparison Notes

`jngen` remains an older and rich generator library with many established ideas.
`CP-Stress-Gen` takes a smaller C++17-focused direction: explicit RNG objects, direct
`g++` checks, CI, bilingual documentation, composable builders, and examples built
around local stress-test workflows.

## Development Checks

Tests are plain C++ programs using `assert` and direct `g++` commands:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/test_random.cpp -o .tmp_build/test_random.exe
.\.tmp_build\test_random.exe
```

Run the full local check from PowerShell:

```powershell
.\scripts\check_all.ps1
```

The full check compiles all tests, runs all test executables, and compiles all examples
using direct `g++` commands. GitHub Actions CI uses the same direct `g++` approach and
does not use CMake.

## License

MIT. See `LICENSE`.

[ci-badge]: https://github.com/tutithuybi123/CP-Stress-Gen/actions/workflows/ci.yml/badge.svg
[ci-workflow]: https://github.com/tutithuybi123/CP-Stress-Gen/actions/workflows/ci.yml
[license-badge]: https://img.shields.io/badge/License-MIT-yellow.svg
[cpp17-badge]: https://img.shields.io/badge/C%2B%2B-17-blue.svg
[cpp17-reference]: https://en.cppreference.com/w/cpp/17
