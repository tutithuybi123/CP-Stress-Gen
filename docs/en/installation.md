# Installation

CP-Stress-Gen is header-only. There is no package manager, CMake project, or external
library to install. Download the repository, then compile your generator with either
`include/` or the generated single-header bundle in `dist/`.

## Requirements

- A C++17 compiler.
- `g++` for the commands below.
- PowerShell if you want to run the bundled scripts on Windows.

## Download

Clone with Git:

```powershell
git clone https://github.com/tutithuybi123/CP-Stress-Gen.git
cd CP-Stress-Gen
```

Or download the ZIP from GitHub:

1. Open `https://github.com/tutithuybi123/CP-Stress-Gen`.
2. Choose `Code` -> `Download ZIP`.
3. Extract the ZIP file.
4. Open PowerShell in the extracted `CP-Stress-Gen` folder.

## Verify The Layout

The source-of-truth headers are:

```text
include/cp_stress_gen.hpp
include/core/
include/modules/
include/anti/
```

The generated single-header bundle is:

```text
dist/cp_stress_gen.hpp
```

## Use Method 1: Include Directory

Use this when you keep the whole repository available:

```powershell
New-Item -ItemType Directory -Force .tmp_build
g++ -std=c++17 -Wall -Wextra -pedantic `
    -Iinclude examples/easy_array_sum.cpp `
    -o .tmp_build/easy_array_sum.exe
.\.tmp_build\easy_array_sum.exe
```

From a sibling project:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic `
    -I..\CP-Stress-Gen\include generator.cpp `
    -o generator.exe
```

## Use Method 2: Single Header

Use this when you want to copy one file into a contest template:

```cpp
#include "cp_stress_gen.hpp"
```

Compile against `dist/`:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic `
    -Idist examples/single_header_basic.cpp `
    -o .tmp_build/single_header_basic.exe
```

You can copy `dist/cp_stress_gen.hpp` into your personal template folder and compile
with that folder on the include path.

## Update The Bundle

`include/` is authoritative. Regenerate `dist/cp_stress_gen.hpp` after public header
changes:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\bundle_single_header.ps1
```

## Minimal Generator

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

Compile with either `-Iinclude`, `-Idist`, or the path to your copied header.

## Troubleshooting

- `cp_stress_gen.hpp: No such file or directory`: the include path after `-I` is wrong.
- C++ syntax or `if constexpr` errors: add `-std=c++17`.
- PowerShell script blocked: run with `-ExecutionPolicy Bypass` as shown above.
- `.tmp_build/`, `.exe`, `.inp`, and `.out` files are local outputs and should not be
  committed.
