# Installation

`CP-Stress-Gen` is header-only. Installing it means downloading the project and compiling your generator with the correct include path.

## Requirements

- A C++17 compiler.
- `g++` if you want to use the commands below directly.
- No external libraries.

## Download The Project

Option 1: clone with Git.

```powershell
git clone https://github.com/tutithuybi123/CP-Stress-Gen.git
cd CP-Stress-Gen
```

Option 2: download ZIP from GitHub.

1. Open `https://github.com/tutithuybi123/CP-Stress-Gen`.
2. Choose `Code` -> `Download ZIP`.
3. Extract the ZIP file.
4. Open PowerShell in the extracted `CP-Stress-Gen` folder.

## Verify The Layout

The project should contain:

```text
include/cp_stress_gen.hpp
include/core/
include/modules/
include/anti/
```

The umbrella header is:

```cpp
#include "cp_stress_gen.hpp"
```

## Method 1: Compile Inside This Project

From the `CP-Stress-Gen` folder:

```powershell
New-Item -ItemType Directory -Force .tmp_build
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/easy_array_sum.cpp -o .tmp_build/easy_array_sum.exe
.\.tmp_build\easy_array_sum.exe
```

`-Iinclude` tells the compiler where to find `cp_stress_gen.hpp`.

## Method 2: Use From Another Project

Keep `CP-Stress-Gen` next to your own project:

```text
workspace/
  CP-Stress-Gen/
  MyGenerator/
    generator.cpp
```

Compile from `MyGenerator/`:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -I..\CP-Stress-Gen\include generator.cpp -o generator.exe
.\generator.exe
```

You can also copy only the `include/` folder into your own project and compile with that copied include path.

## Minimal Working Generator

Create `generator.cpp`:

```cpp
#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(123);
    cp_stress_gen::core::Printer out(std::cout);

    auto values = cp_stress_gen::Array(8).range(1, 100).build(rng);

    out.line(values.size());
    out.vector(values);
    return 0;
}
```

Compile it from a folder that has access to `CP-Stress-Gen/include`:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -I..\CP-Stress-Gen\include generator.cpp -o generator.exe
.\generator.exe
```

## Troubleshooting

- `cp_stress_gen.hpp: No such file or directory`: the include path is wrong. Check the path passed after `-I`.
- C++ language or `if constexpr` errors: compile with `-std=c++17` or newer.
- Windows path issues: use PowerShell-friendly relative paths such as `..\CP-Stress-Gen\include`.
- `.tmp_build/` is local build output. Do not commit it.

