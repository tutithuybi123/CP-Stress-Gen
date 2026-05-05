# CP-Stress-Gen

`CP-Stress-Gen` is a lightweight, header-only C++ library for competitive programming test generation. It focuses on practical fluent generators, deterministic seeded output, and simple integration into local stress-test tools.

## Status

Phase 1 is in progress. The current focus is a stable C++17 public API for arrays, trees, graphs, strings, permutations, math helpers, integer geometry helpers, safe adversarial patterns, validation, printing, examples, and plain assert tests.

Planned but not implemented in Phase 1: GUI integration, package manager integration, hash collision generators, Dijkstra-specific adversarial generators, and advanced geometry such as guaranteed convex polygon generation.

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
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/array_basic.cpp -o .tmp_build/array_basic.exe
```

## Current API Direction

Randomized generators should use one explicit random source:

```cpp
cp_stress_gen::core::Random rng(42);
auto graph = cp_stress_gen::Graph(10).edges(20).directed().dag().build(rng);
```

Deterministic generators may expose `build()` when they do not need randomness.

## Examples

The `examples/` directory is planned to contain short standalone examples:

- arrays
- strings
- permutations
- trees
- graphs and DAGs
- integer geometry
- safe adversarial patterns
- printer helpers

## Tests

Tests are plain C++ programs using `assert`, compiled directly with `g++`:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/test_random.cpp -o .tmp_build/test_random.exe
.tmp_build/test_random.exe
```

## Comparison

`testlib.h` is a mature contest validator/interactor toolkit. `jngen` is a capable generator library with many established ideas. `CP-Stress-Gen` is intentionally smaller: it aims to provide a clean, header-only C++17 generator API that is easy to embed in local stress testing workflows.

## Roadmap

See `docs/roadmap.md` for Phase 1 tasks and priorities.

## License

TODO: choose and add a project license before public reuse.

