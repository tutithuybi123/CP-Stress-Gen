# Contributing

Thanks for helping improve `CP-Stress-Gen`. Keep changes small, testable, and compatible with the existing header-only C++17 design.

## Local Setup

Clone the project:

```powershell
git clone https://github.com/tutithuybi123/CP-Stress-Gen.git
cd CP-Stress-Gen
```

Requirements:

- C++17 compiler.
- `g++` available in `PATH`.
- PowerShell for the local scripts.
- No external libraries.

## Checks

Run the full local check before committing:

```powershell
.\scripts\check_all.ps1
```

This compiles all tests, runs all tests, and compiles all examples with:

```text
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude
```

Tests should print visible `[PASS]` output.

## Style Rules

- Keep the library header-only.
- Keep C++17 compatibility.
- Do not add CMake or external dependencies.
- Prefer small focused headers and examples.
- Preserve existing fluent API style.
- Validate invalid generator configurations with clear exceptions.
- Keep randomized generators deterministic when a `core::Random&` is passed to `build(rng)`.
- Avoid overclaiming adversarial helpers; names should describe input shapes.

## Documentation

Update docs when behavior, setup, examples, or public APIs change. Keep English and Vietnamese documentation consistent where both apply.

## Pull Requests And Issues

- Include the motivation for the change.
- Include tests for new behavior.
- Mention the exact local check command used.
- Do not commit generated artifacts such as `.tmp_build/`, `.exe`, `.o`, or `.obj`.
