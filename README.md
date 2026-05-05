# CP-Stress-Gen

`CP-Stress-Gen` is a lightweight, header-only C++17 library for competitive programming test generation. It provides practical fluent generators, deterministic seeded output, validation helpers, and simple output utilities for local stress-testing workflows.

## Status

Phase 1 is implemented and passing with direct `g++` checks. The library currently includes core helpers, arrays, trees, graphs, strings, permutations, math helpers, integer geometry helpers, safe adversarial pattern modules, examples, and plain `assert` tests.

Phase 2 focuses on documentation quality, realistic stress workflow examples, safe generator improvements, stronger validation coverage, and direct-use packaging notes without adding CMake or external dependencies.

## Requirements

- C++17 or newer
- A standard C++ compiler such as `g++`
- No external libraries

## Direct Include Usage

The project is header-only. Add `CP-Stress-Gen/include` to your include path and include the umbrella header:

```cpp
#include "cp_stress_gen.hpp"
```

Compile a file directly:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/array_basic.cpp -o .tmp_build/array_basic.exe
```

Run it:

```powershell
.\.tmp_build\array_basic.exe
```

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

## Module Overview

| Area | Headers | Current capabilities |
| --- | --- | --- |
| Core | `Random.hpp`, `Validate.hpp`, `Printer.hpp` | Xoshiro256**, validation exceptions, vector/edge printing |
| Arrays | `Array.hpp` | ranges, fill, iota, shuffle |
| Trees | `Tree.hpp` | bamboo, star, random, binary, caterpillar, deep-recursion, weighted edges |
| Graphs | `Graph.hpp` | DAG, layered DAG, dense, sparse connected, complete, cycle, bipartite |
| Strings | `String.hpp` | alphabet, lowercase, uppercase, digits, binary, char ranges |
| Permutations | `Permutation.hpp` | identity, shuffle, reversed, almost sorted |
| Math | `Math.hpp` | gcd, lcm, divisors, primes, random prime |
| Geometry | `Geometry.hpp` | integer rectangle points, unique grid points, collinear points |
| Anti patterns | `include/anti/*.hpp` | safe sort/tree/graph/string pattern generators |

## Design Principles

- Header-only and dependency-free.
- C++17 baseline for broad compiler support.
- Fluent APIs for readable generators.
- Explicit deterministic randomness through `build(rng)`.
- Clear exceptions for invalid generator configurations.
- Small, focused headers that are easy to embed in contest tooling.

## Examples

Existing examples demonstrate individual modules:

- `examples/array_basic.cpp`
- `examples/string_basic.cpp`
- `examples/permutation_basic.cpp`
- `examples/tree_basic.cpp`
- `examples/graph_basic.cpp`
- `examples/dag_basic.cpp`
- `examples/geometry_basic.cpp`
- `examples/anti_basic.cpp`
- `examples/printer_basic.cpp`

Practical workflow examples:

- `examples/stress_pair_basic.cpp`: in-process brute/optimized comparison skeleton.
- `examples/testcase_batch.cpp`: prints multiple test cases in one stream.
- `examples/graph_suite.cpp`: connected, DAG, bipartite, and dense graph generation.
- `examples/tree_suite.cpp`: bamboo, star, random, caterpillar, and deep recursion trees.

## Development Checks

Tests are plain C++ programs using `assert`, compiled directly with `g++`:

```powershell
New-Item -ItemType Directory -Force .tmp_build
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/test_random.cpp -o .tmp_build/test_random.exe
.\.tmp_build\test_random.exe
```

Compile examples the same way:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/graph_basic.cpp -o .tmp_build/graph_basic.exe
```

## Known Limitations

- No CMake or package manager integration yet.
- No GUI or `TestSuite-Studio` bridge yet.
- No guaranteed convex polygon generator yet.
- Large-range prime generation is intentionally simple.
- Advanced adversarial generators such as hash collision or Dijkstra-specific patterns are deferred.

## Comparison

`testlib.h` is a mature contest validator/interactor toolkit. `jngen` is a capable generator library with many established ideas. `CP-Stress-Gen` is intentionally smaller: it aims to provide a clean, header-only C++17 generator API that is easy to embed in local stress testing workflows.

## Roadmap

See `docs/roadmap.md`.

## License

MIT. See `LICENSE`.
