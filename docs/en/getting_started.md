# Getting Started

The library is header-only. Add `include/` to your compiler include path and include the umbrella header.

If you have not downloaded the project yet, start with the [installation guide](installation.md).

```cpp
#include "cp_stress_gen.hpp"
```

## First Generator

```cpp
#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(123);
    auto a = cp_stress_gen::Array(5).range(1, 10).build(rng);

    cp_stress_gen::core::Printer out(std::cout);
    out.line(a.size());
    out.vector(a);
}
```

Compile from the project root:

```powershell
New-Item -ItemType Directory -Force .tmp_build
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/easy_array_sum.cpp -o .tmp_build/easy_array_sum.exe
```

## Deterministic Output

Pass the same seed to `core::Random` to reproduce the same randomized result.

```cpp
cp_stress_gen::core::Random rng(2026);
auto edges = cp_stress_gen::Graph(6).edges(7).sparse_connected().build(rng);
```

Use the convenience `build()` overload only for quick exploratory generators where reproducibility is not important.

## Invalid Configurations

Invalid ranges, impossible edge counts, empty alphabets, and impossible unique point requests throw `std::invalid_argument`.
