# Generator Cookbook

These recipes are small starting points for real stress-test generators. Each snippet
assumes:

```cpp
cp_stress_gen::core::Random rng(seed);
cp_stress_gen::core::Printer out(std::cout);
```

## Arrays With Many Duplicates

```cpp
auto values = cp_stress_gen::Array(n).many_equal(3).build(rng);
out.line(n);
out.vector(values);
```

## Bamboo Tree For Recursion Depth

```cpp
auto edges = cp_stress_gen::Tree(n).bamboo().build(rng);
out.edges(n, edges);
```

## Layered DAG

```cpp
auto edges = cp_stress_gen::Graph(n).edges(m).layered_dag(4).build(rng);
out.edges(n, edges);
```

## Multiple Components

```cpp
auto edges = cp_stress_gen::Graph(n).edges(m).connected_components(3).build(rng);
out.edges(n, edges);
```

## Bipartite Graph

```cpp
auto edges = cp_stress_gen::Graph(n).edges(m).bipartite(n / 2).build(rng);
out.edges(n, edges);
```

## KMP And Z String Patterns

```cpp
auto kmp = cp_stress_gen::String::kmp_worst_prefix(n);
auto z = cp_stress_gen::String::anti_z(n);
out.line(kmp);
out.line(z);
```

## Geometry Points And Convex Hull

```cpp
auto points = cp_stress_gen::Geometry::points(n).rectangle(-100, -100, 100, 100).unique().build(rng);
auto hull = cp_stress_gen::Geometry::convex_hull(points);
out.line(points.size(), hull.size());
```

## GCD And Coprime Math Cases

```cpp
auto pair = cp_stress_gen::Math::with_gcd(12, 1, 100, rng);
auto coprime = cp_stress_gen::Math::random_coprime_pair(1, 100, rng);
out.line(pair.first, pair.second);
out.line(coprime.first, coprime.second);
```

## Algorithm-Targeted Anti Patterns

```cpp
auto chain = cp_stress_gen::anti::DfsBfsKiller::deep_chain(n);
auto duplicates = cp_stress_gen::anti::SortKiller::many_duplicates(n, 4, rng);
out.edges(n, chain);
out.vector(duplicates);
```

## Reproducible Options

```cpp
cp_stress_gen::core::Options opt(argc, argv);
int n = opt.get<int>("n", 1000);
long long seed = opt.get<long long>("seed", 1);
```

## Printer Edge Headers

```cpp
out.edges(n, edges);
out.indexed(1).edges(n, edges);
```

## Single-Header Bundle

Use `dist/cp_stress_gen.hpp` when copying the library into a personal template:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Idist generator.cpp -o generator.exe
```
