# CP-Stress-Gen

`CP-Stress-Gen` is a lightweight, header-only C++17 library for competitive programming test generation. It provides fluent generators, deterministic seeded output, validation helpers, simple printers, and practical examples for local stress testing.

## Documentation

- [English documentation](docs/en/overview.md)
- [Tài liệu tiếng Việt](docs/vn/overview.md)

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
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/easy_array_sum.cpp -o .tmp_build/easy_array_sum.exe
.\.tmp_build\easy_array_sum.exe
```

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

Existing focused examples remain available in `examples/`. See [English examples](docs/en/examples.md) or [ví dụ tiếng Việt](docs/vn/examples.md) for compile commands and usage notes.

## Current Modules

| Area | Capabilities |
| --- | --- |
| Core | Xoshiro256**, validation helpers, vector and edge printing |
| Arrays | Ranges, fill, iota, shuffle, almost sorted, many equal, blocky |
| Strings | Alphabets, ranges, palindrome, almost palindrome, periodic, prefix-heavy |
| Permutations | One-based/zero-based, shuffle, reversed, cyclic shift, few swaps, blocks |
| Trees | Bamboo, star, random, binary, caterpillar, deep recursion, weighted edges |
| Graphs | General, DAG, layered DAG, dense, sparse connected, complete, cycle, path, forest, components, bipartite |
| Math | gcd, lcm, divisors, primes, primality, random prime/composite, coprime pairs |
| Geometry | Integer points, unique grid points, collinear, boundary, clustered, duplicates |
| Anti patterns | Safe sort/tree/graph/string patterns with honest names |

## Known Limitations

- No CMake or package manager integration yet.
- No GUI or `TestSuite-Studio` bridge yet.
- No guaranteed convex polygon generator yet.
- Large-range prime generation is intentionally simple.
- Advanced adversarial generators such as hash collision or Dijkstra-specific patterns are deferred.

## Development Checks

Tests are plain C++ programs using `assert` and direct `g++` commands:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/test_random.cpp -o .tmp_build/test_random.exe
.\.tmp_build\test_random.exe
```

## License

MIT. See `LICENSE`.
