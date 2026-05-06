# Benchmarks

CP-Stress-Gen includes small benchmark examples for local experiments. They use
`std::chrono` and print elapsed milliseconds plus a checksum so the generated data is
observed by the program.

These examples are not universal performance claims. Results depend on the compiler,
machine, optimization flags, and chosen generator mode.

## Array Benchmark

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -Iinclude examples/benchmark_array.cpp -o .tmp_build/benchmark_array.exe
.\.tmp_build\benchmark_array.exe --n 100000 --seed 1
```

## Graph Benchmark

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -Iinclude examples/benchmark_graph.cpp -o .tmp_build/benchmark_graph.exe
.\.tmp_build\benchmark_graph.exe --n 1000 --m 2000 --seed 1
```

## String Benchmark

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -Iinclude examples/benchmark_string.cpp -o .tmp_build/benchmark_string.exe
.\.tmp_build\benchmark_string.exe --n 100000 --seed 1
```

Use smaller defaults while editing examples, then increase sizes only when you are
measuring a specific generator on your own machine.
