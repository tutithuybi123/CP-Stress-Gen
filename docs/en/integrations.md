# Integration Workflows

CP-Stress-Gen does not depend on Polygon, Themis, or any local judge runner. The examples
in this section are portable generator patterns that compile with normal `g++`.

## Polygon-Style Generator

Polygon generators usually print one test case to standard output. Use:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/polygon_generator.cpp -o .tmp_build/polygon_generator.exe
.\.tmp_build\polygon_generator.exe --n 20 --m 30 --seed 1 --mode graph
```

The example accepts `--mode graph`, `--mode dag`, and `--mode bipartite`.

## Themis-Style Generator

Themis workflows often use `.inp` files. The example can print to stdout or write files:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/themis_generator.cpp -o .tmp_build/themis_generator.exe
.\.tmp_build\themis_generator.exe --tests 3 --n 20 --seed 1 --out case
```

Generated `.inp` files are local outputs and should not be committed.

## Local Stress Driver

`examples/local_stress_driver.cpp` keeps the generator, brute solution, and optimized
solution in one file. It does not run external programs:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude examples/local_stress_driver.cpp -o .tmp_build/local_stress_driver.exe
.\.tmp_build\local_stress_driver.exe --tests 200 --n 30 --seed 1
```

## Single-Header Use

For copy-only contest templates, compile with `dist/`:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Idist examples/single_header_basic.cpp -o .tmp_build/single_header_basic.exe
```
