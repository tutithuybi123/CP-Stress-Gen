# Library Build And Include Usage

`CP-Stress-Gen` does not require a build system. It is a collection of headers under `include/`.

For download and setup steps, see the [installation guide](installation.md).

## Direct Compile

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/graph_basic.cpp -o .tmp_build/graph_basic.exe
```

The important flag is:

```text
-Iinclude
```

That exposes:

```cpp
#include "cp_stress_gen.hpp"
```

## Copy Into Another Project

Copy the `include/` directory into your project and compile with the copied include path.

```powershell
g++ -std=c++17 -Ipath\to\CP-Stress-Gen\include generator.cpp -o generator.exe
```

## Current Packaging Status

There is no CMake target, package manager recipe, or installed system package yet. Keep usage explicit with a compiler include path.
